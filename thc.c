#include "thc.h"

#include <string.h>

#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


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

void print_escaped(const char *s) {
  for (; *s; s++) {
    switch (*s) {
    case '&':
      fputs("&amp;", stdout);
      break;
    case '>':
      fputs("&gt;", stdout);
      break;
    case '<':
      fputs("&lt;", stdout);
      break;
    case '"':
      fputs("&quot;", stdout);
      break;
    default:
      putc(*s, stdout);
    }
  }
}

void print_tree(struct node *tree, int depth) {
  if (tree->type == TEXT) {
    print_escaped(tree->data.text);
  } else {
    struct list *list;
    printf("<%s", tree->data.tag.name);
    list = tree->data.tag.attrs;
    while (list) {
      printf(" ");
      print_escaped(((struct attr*)list->data)->key);
      printf("=\"");
      print_escaped(((struct attr*)list->data)->val);
      printf("\"");
      list = list->next;
    }
    if ( tree->data.tag.children == NULL )
      printf(" />\n");
    else {
      int hastag = 0;
      int wastext = 0;
      printf(">");
      /* verify that the tag's children contain at least one tag,
       * i.e. not a textnode.  Verify this by checking the first
       * and/or the second child. */
      if (((struct node*)tree->data.tag.children->data)->type != TEXT
          || (tree->data.tag.children->next != NULL
            && ((struct node*)tree->data.tag.children->next->data)->type != TEXT))
        hastag = 1;
      if ( hastag==1 )
        printf("\n");
      list = tree->data.tag.children;
      while (list) {
        if ( hastag==1 && wastext==0 )
          pad(depth+2);
        print_tree((struct node*)list->data, depth+2);
        if ( ((struct node*)list->data)->type == TEXT )
          wastext = 1;
        else
          wastext = 0;
        list = list->next;
      }
      if ( hastag==1 && wastext==0 )
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
static char* website_pages[][WEBSITEPAGESELSPERROW] = {
  { "Front", "Forsiden", "./" },
  { "Videos", "Videoer", "./videoer.thc" },
  { "About", "Om", "./om.thc" },
  { "Now", "Nu", "./nu.thc" },
  { "Dictionary", "Ordbog", "./ordbog.thc" },
  { "FanFiction", "Fanfiction", "./fanfiction.thc" }
};

void webpage_start(struct html_builder *builder,
                   const char* page_name,
                   const char* title) {
  char *newtitle;
  int i;
  if ( title == NULL )
    newtitle = "topDatamat";
  else
    newtitle = printbf("topDatamat / %s", title);
  
  html_builder_init(builder, "html", NULL);
  
  TAG(("head"),
      TAG(("title"), TEXT(newtitle))
      TAG(("link", "href", "/media/styles.css", "rel", "stylesheet"),));
  /* In case of a fatal site performance, make the entire page know */
  enter_tag(builder, "body", page_name != NULL ? NULL : "id", "page-fatal", NULL);
  TAG(("header", "id", "header"),
      TAG(("div", "id", "viewsource"),
          TAG(("a", "href", printbf("./%s", source_file)), TEXT("Se koden nøgen")))
      if ( page_name != NULL ) {
        /*TAG(("div", "id", "languages"),);*/
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
  enter_tag(builder, "iframe",
            "width", printbf("%d", width),
            "height", printbf("%d", height),
            "src", printbf("http://www.youtube.com/embed/%s", watchid),
            "frameborder", "0", "allowfullscreen", "true", NULL);
  insert_text(builder, "");
  leave_tag(builder); /* iframe */
}

char *youtube_url(char *watchid) {
  return printbf("http://youtube.com/watch?v=%s", watchid);
}

size_t term_size(struct term *term) {
  return strlen(term->term)+1
    + (term->abbr ? strlen(term->abbr)+1 : 0)
    + strlen(term->translation)+1;
}

int write_dictionary(const char *path, struct dictionary *dict) {
  int fd = open(path, O_CREAT | O_RDWR | O_TRUNC,
                S_IRUSR | S_IWUSR | S_IROTH | S_IRGRP);
  long str_offset = sizeof(*dict) + dict->num_terms * sizeof(struct term);

  if (-1 == fd) {
    return -1;
  }

  long dict_size = str_offset;

  for (size_t i = 0; i < dict->num_terms; i++) {
    dict_size += term_size(dict->terms+i)+1;
  }

  lseek(fd, dict_size - 1, SEEK_SET);

  if (write(fd, "", 1) != 1) {
    return -1;
  }

  void *orig = mmap(NULL, dict_size,
                    PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
  char *loc = (char*)orig;
  struct dictionary *newdict = (struct dictionary*)loc;
  struct term *newterms = (struct term*)(loc+sizeof(*newdict));
  newdict->this = newdict;
  newdict->num_terms = dict->num_terms;
  newdict->terms = newterms;
  loc = (char*)newterms;

  for (size_t i = 0; i < dict->num_terms; i++) {
    struct term *from = dict->terms+i;
    struct term *to = newterms+i;
    strcpy(to->term=loc+str_offset, from->term);
    str_offset += strlen(from->term)+1;
    if (from->abbr) {
      strcpy(to->abbr=loc+str_offset, from->abbr);
      str_offset += strlen(from->abbr)+1;
    }
    strcpy(to->translation=loc+str_offset, from->translation);
    str_offset += strlen(from->translation)+1;
  }

  munmap(orig, dict_size);
  close(fd);
  return 0;
}

struct dictionary* read_dictionary(const char *path) {
  struct dictionary dict;
  struct stat statbuf;
  int fd = open(path, O_RDWR);

  if (fd == -1) {
    return NULL;
  }

  fstat (fd,&statbuf);
  if (read(fd, &dict, sizeof(dict)) != sizeof(dict)) {
    return NULL;
  }
  dict.this = mmap((void*)dict.this, statbuf.st_size,
                   PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0);
  close(fd);
  if (dict.this == (void*)-1) {
    return NULL;
  } else {
    return dict.this;
  }
}

void strappend(char *dest, const char *s) {
  strcpy(dest+strlen(dest), s);
}

char* printbf(const char *s, ...) {
  va_list vl;
  int bufsiz = 16;
  char *buf = NULL;
  int wr;
  do {
    free(buf);
    buf = malloc(bufsiz *= 2);
    va_start(vl,s);
    wr = vsnprintf(buf, bufsiz, s, vl);
  } while (wr >= bufsiz);
  return buf;
}
