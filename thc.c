#include "thc.h"

#include <unistd.h>
#include <signal.h>
#include <time.h>

void signal_insertrow(struct html_builder *builder,
                      char *signal_name,
                      char *signal_code,
                      char *value) {
  TAG(("tr"),
      TAG(("td"),
          TEXT(signal_name);
          TAG(("tt"), TEXT(signal_code)))
      TAG(("td"), TEXT(value)))
    }

void signal_handler(int signal, siginfo_t *info, void *context) {
  char *error, *tmp = malloc(64);
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  UNUSED(info);
  UNUSED(context);
  switch (signal) {
  case SIGSEGV:
    error = "Segmentation fault";
    break;
  case SIGBUS:
    error = "Bus error";
    break;
  default:
    error = "Unhandled exception";
  }
  webpage_start(builder, NULL, "Fejl");
  TAG(("article"),
      TAG(("header"),
          TAG(("h1"),
              sprintf(tmp, "Fejl: %s", error);
              TEXT(tmp)));
      TAG(("table", "class" "error-table"),
          TAG(("thead"),
              TAG(("tr"),
                  TAG(("th"), TEXT("Variabel"));
                  TAG(("th"), TEXT("Værdi"))));
          TAG(("tbody"),
              tmp = malloc(32); sprintf(tmp, "%d", info->si_signo);
              signal_insertrow(builder, "Signal number", "si_signo", tmp);
              tmp = malloc(32); sprintf(tmp, "%d", info->si_errno);
              signal_insertrow(builder, "Error number", "si_errno", tmp);
              tmp = malloc(32); sprintf(tmp, "%d", info->si_code);
              signal_insertrow(builder, "Signal code", "si_code", tmp);
              /*
                tmp = malloc(32); sprintf(tmp, "%d", info->si_trapno);
                signal_insertrow(builder, "Trap number", "si_trapno", tmp);
              */
              tmp = malloc(32); sprintf(tmp, "%d", info->si_pid);
              signal_insertrow(builder, "Process id", "si_pid", tmp);
              tmp = malloc(32); sprintf(tmp, "%d", info->si_uid);
              signal_insertrow(builder, "User id", "si_uid", tmp);
              tmp = malloc(32); sprintf(tmp, "%d", info->si_status);
              signal_insertrow(builder, "Exit status", "si_status", tmp);
              tmp = malloc(32); sprintf(tmp, "%f", ((float)info->si_utime/CLOCKS_PER_SEC));
              signal_insertrow(builder, "User time", "si_utime", tmp);
              tmp = malloc(32); sprintf(tmp, "%f", ((float)info->si_stime/CLOCKS_PER_SEC));
              signal_insertrow(builder, "System time", "si_stime", tmp);
              /*
                tmp = malloc(32); sprintf(tmp, "%f", (float)info->si_value);
                signal_insertrow(builder, "Signal value", "si_value", tmp);
              */
              tmp = malloc(32); sprintf(tmp, "%d", info->si_int);
              signal_insertrow(builder, "POSIX signal", "si_int", tmp);
              )));
  webpage_end(builder);
  print_tree(builder->top_node, 0);
  exit(1);
}

int main(int argc, char** argv) {
  struct sigaction action;
  action.sa_sigaction = signal_handler;
  action.sa_flags = SA_SIGINFO;
  sigemptyset(&action.sa_mask);
  sigaction(SIGSEGV, &action, NULL);
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
  struct node *node = NULL;
  for (key=va_arg(vl,char*); key; key=va_arg(vl,char*)) {
    struct list *cons = malloc(sizeof(struct list));
    val=va_arg(vl,char*);
    cons->data = malloc(sizeof(struct attr));
    ((struct attr*)cons->data)->key = key;
    ((struct attr*)cons->data)->val = val;
    cons->next = list;
    list = cons;
  }
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
  va_end(vl);

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
  va_end(vl);
  new_cons = list_cons(new_node, NULL);
  if (*builder->last_node) {
    (*builder->last_node)->next = new_cons;
    builder->last_node = &new_cons->next;
  } else {
    *builder->last_node = new_cons;
  }
  builder->stack = list_cons(builder->last_node, builder->stack);
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
  va_end(vl);
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
    builder->last_node = &((*builder->last_node)->next->next);
  } else {
    *(builder->last_node) = list_cons(new_node, NULL);
  }
}

void insert_texts(struct html_builder *builder,
                  ...) {
  va_list vl;
  va_start(vl,builder);
  char *text;
  while ((text = va_arg(vl,char*))) {
    insert_text(builder, text);
  }
  va_end(vl);
}

/* The 'template' system for the webpage. */

/* List of pages */
#define WEBSITEPAGESELSPERROW 3
static char* website_pages[4][WEBSITEPAGESELSPERROW] = {
  { "Front", "Forsiden", "./" },
  { "Videos", "Videoer", "./videoer.thc" },
  { "About", "Om", "./om.thc" },
  { "Dictionary", "Ordbog", "./ordbog.thc" }
};

void webpage_start(struct html_builder *builder,
                   const char* page_name,
                   const char* title) {
  char *newtitle = malloc(64), *tmp = malloc(32);
  int i;
  if ( title == NULL )
    newtitle = "topDatamat";
  else
    sprintf(newtitle, "topDatamat / %s", title);
  
  html_builder_init(builder, "html", NULL);
  
  TAG(("head"),
      TAG(("title"), TEXT(newtitle))
      TAG(("link", "href", "/media/styles.css", "rel", "stylesheet"),));
  /* In case of a fatal site performance, make the entire page know */
  enter_tag(builder, "body", page_name != NULL ? NULL : "id", "page-fatal", NULL);
  TAG(("header", "id", "header"),
      TAG(("div", "id", "viewsource"),
          sprintf(tmp, "./%s", __FILE__);
          TAG(("a", "href", tmp), TEXT("Se koden nøgen")))
      if ( page_name != NULL ) {
        TAG(("div", "id", "languages"),);
        TAG(("div", "id", "indbakken"),
            TEXT("Send dine spørgsmål til ");
            TAG(("a", "href", "mailto:indbakken@topdatamat.dk"),
                TEXT("indbakken@topdatamat.dk"))
            TEXT(",");
            TAG(("br"),);
            TEXT("så svarer Troels måske på dem.");
            )
          }
      TAG(("h1"), TEXT("topDatamat"))
      TAG(("ul", "id", "menu"),
          for ( i = 0; i < (int)(sizeof(website_pages)/
                                 sizeof(char*)/
                                 WEBSITEPAGESELSPERROW); i++ ) {
            TAG(("li", website_pages[i][0] != page_name ? NULL : "class", "highlighted"),
                TAG(("a", "href", website_pages[i][2]),
                    TEXT(website_pages[i][1])))
              }))
    enter_tag(builder, "div", "id", "content", NULL);
}

void webpage_end(struct html_builder *builder) {
  leave_tag(builder); /* div#content */
  TAG(("footer", "id", "footer"),
      TAG(("p"), TEXT("Denne hjemmeside er skrevet i C.")))
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
