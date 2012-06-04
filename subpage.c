#include "thc.h"

#include <stdio.h>

int pagemain(int argc, char** argv) {
  struct html_builder builder;
  UNUSED(argc);
  UNUSED(argv);
  
  webpage_start(&builder, "Underside");
  
  enter_tag(&builder, "p", NULL);
  insert_text(&builder, "Underside!");
  leave_tag(&builder);
  
  webpage_end(&builder);

  print_tree(builder.top_node, 0);

  return 0;
}
