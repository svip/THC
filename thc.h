#ifndef THC_H
#define THC_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define THC_VERSION "1.0"

#define UNUSED(x) (x=x)

struct attr {
  const char *key;
  const char *val;
};

struct list {
  void *data;
  struct list *next;
};

struct list* list_cons(void* data, struct list *list);

enum node_type { TEXT, TAG };

struct tag_node {
  const char *name;
  struct list *attrs;
  struct list *children;
};

struct node {
  char type;
  union {
    struct tag_node tag;
    const char *text;
  } data;
};

void print_tree(struct node *tree, int depth);

struct html_builder {
  struct list *stack;
  struct node *top_node;
  struct list **last_node;
};

int html_builder_init(struct html_builder *builder,
                      const char *name,
                      ...);

void enter_tag(struct html_builder *builder,
               const char *name,
               ...);

void leave_tag(struct html_builder *builder);

void append_tag(struct html_builder *builder,
                const char *name,
                ...);

void insert_text(struct html_builder *builder,
                 const char* text);

int pagemain(int argc, char** argv);

#endif
