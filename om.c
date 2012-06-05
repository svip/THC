#include "thc.h"

#include <stdio.h>

char *website_pagename = "om";

int pagemain(int argc, char** argv) {
  struct html_builder builder;
  UNUSED(argc);
  UNUSED(argv);
  
  webpage_start(&builder, "About", "Om");
  
  enter_tag(&builder, "article", NULL);
  enter_tag(&builder, "header", NULL);
  enter_tag(&builder, "h1", NULL);
  insert_text(&builder, "Om");
  leave_tag(&builder); /* h1 */
  enter_tag(&builder, "h6", NULL);
  insert_text(&builder, "English below");
  leave_tag(&builder); /* h6 */
  leave_tag(&builder); /* header */
  enter_tag(&builder, "article", NULL);
  enter_tag(&builder, "p", NULL);
  insert_text(&builder, "Direkte fra DIKUs kælder kommer topDatamat - tre akavede nørder der forsøger at give et retvisende billede af datalogien.  Følg med når Brainfuck udfører dybdeborende interviews, Troels besvarer datalogiens dybeste spørgsmål og den norske Truls forsøger på at integrere sig i en fremmed kultur.");
  leave_tag(&builder); /* p */
  leave_tag(&builder); /* article */
  enter_tag(&builder, "article", NULL);
  enter_tag(&builder, "header", NULL);
  enter_tag(&builder, "h1", NULL);
  insert_text(&builder, "About");
  leave_tag(&builder); /* h1 */
  leave_tag(&builder); /* header */
  enter_tag(&builder, "p", NULL);
  insert_text(&builder, "topDatamat is a Danish sketch show with computers and IT in focus.");
  leave_tag(&builder); /* p */
  leave_tag(&builder); /* article */
  leave_tag(&builder); /* article */
  
  webpage_end(&builder);

  print_tree(builder.top_node, 0);

  return 0;
}
