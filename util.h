#ifndef UTIL_H
#define UTIL_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define UNUSED(x) (x=x)

struct attr {
  const char *key;
  const char *val;
};

struct list {
  void *data;
  struct list *next;
};

struct list* list_cons(void* data, struct list *list) {
  struct list *new_cons = malloc(sizeof(struct list));
  new_cons->data = data;
  new_cons->next = list;
  return new_cons;
}

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

struct node* mktagnode(const char *name, ...) {
  char *key, *val;
  struct list *list = NULL;
  struct list *cons;
  struct node *node;
  va_list vl;
  va_start(vl,name);

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

struct html_builder {
  struct list *stack;
  struct node *top_node;
  struct list **last_node;
};

int html_builder_init(struct html_builder *builder,
                      struct node *root) {
  if (root->type != TAG) {
    return -1;
  }

  builder->stack = NULL;
  builder->top_node = root;
  builder->last_node = &(root->data.tag.children);
  return 0;
}

void enter_tag(struct html_builder *builder,
               struct node *new_node) {
  struct list *new_cons = list_cons(new_node, NULL);
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
                struct node *new_node) {
  enter_tag(builder, new_node);
  leave_tag(builder);
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

#endif
