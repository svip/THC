#include "thc.h"

#include <stdio.h>

int pagemain(int argc, char** argv) {
  struct html_builder builder;
  UNUSED(argc);
  UNUSED(argv);
  
  webpage_start(&builder, NULL);
  
  enter_tag(&builder, "p", NULL);
  insert_text(&builder, "Hej!");
  leave_tag(&builder);
  
  webpage_end(&builder);

  print_tree(builder.top_node, 0);

  /*  test();*/

  return 0;
}
