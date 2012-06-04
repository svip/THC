#include "thc.h"

#include <stdio.h>

int pagemain(int argc, char** argv) {
  struct html_builder builder;
  html_builder_init(&builder, "html", "version", "bad", NULL);
  UNUSED(argc);
  UNUSED(argv);

  enter_tag(&builder, "head", NULL);
  enter_tag(&builder, "title", NULL);
  insert_text(&builder, "Index page!");
  leave_tag(&builder);
  leave_tag(&builder);
  enter_tag(&builder, "body", NULL);
  enter_tag(&builder, "p", NULL);
  insert_text(&builder, "Hej!");
  leave_tag(&builder);
  leave_tag(&builder);

  print_tree(builder.top_node, 0);

  /*  test();*/

  return 0;
}
