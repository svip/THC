#ifndef UTIL_H
#define UTIL_H

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

void html_builder_init(struct html_builder *builder,
                       const char* name,
                       struct list *attrs) {
  struct node *new_node = malloc(sizeof(struct node));
  new_node->type = TAG;
  new_node->data.tag.name = name;
  new_node->data.tag.attrs = attrs;
  new_node->data.tag.children = NULL;
  builder->stack = NULL;
  builder->top_node = new_node;
  builder->last_node = &(new_node->data.tag.children);
}

void enter_tag(struct html_builder *builder,
               const char* name,
               struct list *attrs) {
  struct node *new_node = malloc(sizeof(struct node));
  struct list *new_cons = list_cons(new_node, NULL);
  new_node->type = TAG;
  new_node->data.tag.name = name;
  new_node->data.tag.attrs = attrs;
  new_node->data.tag.children = NULL;
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
                const char* name,
                struct list *attrs) {
  enter_tag(builder, name, attrs);
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
