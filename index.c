#include "boilerplate.h"
#include "util.h"

#include <stdio.h>

int pagemain(int argc, char** argv) {
  struct html_builder builder;
  html_builder_init(&builder, mktagnode("html", "version", "bad", NULL));
  UNUSED(argc);
  UNUSED(argv);

  enter_tag(&builder, mktagnode("head", NULL));
  enter_tag(&builder, mktagnode("title", NULL));
  insert_text(&builder, "Index page!");
  leave_tag(&builder);
  leave_tag(&builder);
  enter_tag(&builder, mktagnode("body", NULL));
  enter_tag(&builder, mktagnode("p", NULL));
  insert_text(&builder, "Hej!");
  leave_tag(&builder);
  leave_tag(&builder);

  print_tree(builder.top_node, 0);

  /*  test();*/

  return 0;
}
