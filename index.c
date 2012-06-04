#include "thc.h"

#include <stdio.h>

int pagemain(int argc, char** argv) {
  struct html_builder builder;
  UNUSED(argc);
  UNUSED(argv);
  
  webpage_start(&builder, NULL);
  
  enter_tag(&builder, "article", NULL);
  enter_tag(&builder, "header", NULL);
  enter_tag(&builder, "h1", NULL);
  enter_tag(&builder, "b", NULL);
  insert_text(&builder, "topDatamat 0x0");
  leave_tag(&builder); /* b */
  insert_text(&builder, " udgivet! (");
  enter_tag(&builder, "a", "href", "http://youtube.com/watch?v=0ohC89vJjnY", NULL);
  insert_text(&builder, "Click here for English subtitles");
  leave_tag(&builder); /* a */
  insert_text(&builder, ")");
  leave_tag(&builder); /* h1 */
  leave_tag(&builder); /* header */
  enter_tag(&builder, "div", "class", "post", NULL);
  webpage_youtubeembed(&builder, "lgmA9z8Sb5E", 853, 480);
  enter_tag(&builder, "p", NULL);
  insert_text(&builder, "Så er første afsnit af topDatamat her endeligt!");
  leave_tag(&builder); /* p */
  enter_tag(&builder, "p", NULL);
  insert_text(&builder, "Nyd det her eller ");
  enter_tag(&builder, "a", "href", "http://youtube.com/watch?v=lgmA9z8Sb5E", NULL);
  insert_text(&builder, "direkte på YouTube");
  leave_tag(&builder); /* a */
  insert_text(&builder, ".");
  leave_tag(&builder); /* p */
  leave_tag(&builder); /* div */
  leave_tag(&builder); /* article */
  
  webpage_end(&builder);

  print_tree(builder.top_node, 0);

  /*  test();*/

  return 0;
}
