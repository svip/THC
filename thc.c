#include "thc.h"

int main(int argc, char** argv) {
/*  printf("HTTP/1.1 200 OK\r\n");
  printf("Server: THC %s\r\n", THC_VERSION);*/
  printf("Content-type: text/html; charset=utf-8\r\n");
  printf("Cache-control: max-age=3600\r\n");
  printf("\r\n");
  printf("<!doctype html>\r\n");
  return pagemain(argc, argv);
}

struct list* list_cons(void* data, struct list *list) {
  struct list *new_cons = malloc(sizeof(struct list));
  new_cons->data = data;
  new_cons->next = list;
  return new_cons;
}

struct node* mktagnode(const char *name, va_list vl) {
  char *key, *val;
  struct list *list = NULL;
  struct list *cons;
  struct node *node;

  for (key=va_arg(vl,char*); key; key=va_arg(vl,char*)) {
    val=va_arg(vl,char*);
    cons=malloc(sizeof(struct list));
    cons->data = malloc(sizeof(struct attr));
    ((struct attr*)cons->data)->key = key;
    ((struct attr*)cons->data)->val = val;
    cons->next = list;
    list = cons;
  }
  va_end(vl);
  node = malloc(sizeof(struct node));
  node->type = TAG;
  node->data.tag.name = name;
  node->data.tag.attrs = list;
  node->data.tag.children = NULL;
  return node;
}

void pad(int depth) {
  while (depth--) {
    printf(" ");
  }
}

void print_tree(struct node *tree, int depth) {
  if (tree->type == TEXT) {
    pad(depth);
    printf("%s\n", tree->data.text);
  } else {
    struct list *list;
    pad(depth);
    printf("<%s", tree->data.tag.name);
    list = tree->data.tag.attrs;
    while (list) {
      printf(" ");
      printf("%s=\"", ((struct attr*)list->data)->key);
      printf("%s", ((struct attr*)list->data)->val);
      printf("\"");
      list = list->next;
    }
    if ( tree->data.tag.children == NULL )
      printf(" />\n");
    else {
      printf(">\n");
      list = tree->data.tag.children;
      while (list) {
        print_tree((struct node*)list->data, depth+2);
        list = list->next;
      }
      pad(depth);
      printf("</%s>\n", tree->data.tag.name);
    }
  }
}

int html_builder_init(struct html_builder *builder,
                      const char *name,
                      ...) {
  va_list vl;
  struct node *root;
  va_start(vl,name);
  root = mktagnode(name, vl);

  builder->stack = NULL;
  builder->top_node = root;
  builder->last_node = &(root->data.tag.children);
  return 0;
}

void enter_tag(struct html_builder *builder,
               const char *name,
               ...) {
  struct node *new_node;
  struct list *new_cons;
  va_list vl;
  va_start(vl,name);
  new_node = mktagnode(name, vl);
  new_cons = list_cons(new_node, NULL);
  if (*builder->last_node) {
    (*builder->last_node)->next = new_cons;
  } else {
    (*builder->last_node) = new_cons;
  }
  builder->stack = list_cons(&(new_cons->next), builder->stack);
  builder->last_node = &new_node->data.tag.children;
}

void leave_tag(struct html_builder *builder) {
  builder->last_node = (struct list**) builder->stack->data;
  builder->stack = builder->stack->next;
}

void append_tag(struct html_builder *builder,
                const char *name,
                ...) {
  struct node *new_node;
  struct list *new_cons;
  va_list vl;
  va_start(vl,name);
  new_node = mktagnode(name, vl);
  new_cons = list_cons(new_node, NULL);
  if (*builder->last_node) {
    (*builder->last_node)->next = new_cons;

  }
  (*builder->last_node) = new_cons;
}

void insert_text(struct html_builder *builder,
                 const char* text) {
  struct node *new_node = malloc(sizeof(struct node));
  new_node->type = TEXT;
  new_node->data.text = text;
  if (*(builder->last_node)) {
    (*builder->last_node)->next = list_cons(new_node, NULL);
    builder->last_node = &((*builder->last_node)->next);
  } else {
    *(builder->last_node) = list_cons(new_node, NULL);
  }
}

/* The 'template' system for the webpage. */

void webpage_start(struct html_builder *builder,
                   const char* page_name,
                   const char* title) {
  char *newtitle = malloc(64);
  int i;
  if ( title == NULL )
    newtitle = "topDatamat";
  else
    sprintf(newtitle, "topDatamat / %s", title);
  
  html_builder_init(builder, "html", NULL);
  
  enter_tag(builder, "head", NULL);
  enter_tag(builder, "title", NULL);
  insert_text(builder, newtitle);
  leave_tag(builder); /* title */
  enter_tag(builder, "link", "href", "/media/styles.css", "rel", "stylesheet", NULL);
  leave_tag(builder); /* link */
  leave_tag(builder); /* head */
  enter_tag(builder, "body", NULL);
  enter_tag(builder, "header", "id", "header", NULL);
  enter_tag(builder, "div", "id", "languages", NULL);
  leave_tag(builder); /* div#languages */
  enter_tag(builder, "div", "id", "indbakken", NULL);
  insert_text(builder, "Send dine spørgsmål til ");
  enter_tag(builder, "a", "href", "mailto:indbakken@topdatamat.dk", NULL);
  insert_text(builder, "indbakken@topdatamat.dk");
  leave_tag(builder); /* a */
  insert_text(builder, ",");
  enter_tag(builder, "br", NULL);
  leave_tag(builder); /* br */
  insert_text(builder, "så svarer Troels måske på dem.");
  leave_tag(builder); /* div#indbakken */
  enter_tag(builder, "h1", NULL);
  insert_text(builder, "topDatamat");
  leave_tag(builder); /* h1 */
  enter_tag(builder, "ul", "id", "menu", NULL);
  for ( i = 0; i < (int)(sizeof(website_pages)/
                         sizeof(char*)/
                         WEBSITEPAGESELSPERROW); i++ ) {
    if ( website_pages[i][0] == page_name )
      enter_tag(builder, "li", "class", "highlighted", NULL);
    else
      enter_tag(builder, "li", NULL);
    enter_tag(builder, "a", "href", website_pages[i][2], NULL);
    insert_text(builder, website_pages[i][1]);
    leave_tag(builder); /* a */
    leave_tag(builder); /* li */
  }
  leave_tag(builder); /* ul#menu */
  leave_tag(builder); /* header */
  enter_tag(builder, "div", "id", "content", NULL);
}

void webpage_end(struct html_builder *builder) {
  leave_tag(builder); /* div#content */
  enter_tag(builder, "footer", "id", "footer", NULL);
  enter_tag(builder, "p", NULL);
  insert_text(builder, "Denne hjemmeside er skrevet i C.");
  leave_tag(builder); /* p */
  leave_tag(builder); /* footer#footer */
  leave_tag(builder); /* body */
}

void webpage_youtubeembed(struct html_builder *builder,
                          const char* watchid,
                          const int width,
                          const int height) {
  char *url = malloc(64);
  char *strwidth = malloc(4);
  char *strheight = malloc(4);
  sprintf(url, "http://www.youtube.com/embed/%s", watchid);
  sprintf(strwidth, "%d", width);
  sprintf(strheight, "%d", height);
  enter_tag(builder, "iframe", "width", strwidth, "height", strheight,
    "src", url, "frameborder", "0", "allowfullscreen", "true", NULL);
  insert_text(builder, "");
  leave_tag(builder); /* iframe */
}

char *youtube_url(char *watchid) {
  char *tmp = malloc(64);
  sprintf(tmp, "http://youtube.com/watch?v=%s", watchid);
  return tmp;
}
