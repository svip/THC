#include "thc.h"

#include <stdio.h>

int pagemain(int argc, char** argv) {
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  UNUSED(argc);
  UNUSED(argv);
  
  webpage_start(builder, "Front", NULL);

  TAG(("article"),
    TAG(("header"),
      TAG(("h1"),
        TAG(("b"), TEXT("topDatamat 0x1"));
        TEXT(" udgivet !");/* (
        TAG(("a", "href", "http://youtube.com/watch?v=0ohC89vJjnY"),
          TEXT("Click here for English subtitles"))
        TEXT(")")*/));
    TAG(("div", "class", "post"),
      webpage_youtubeembed(builder, "F3HHS50dA6g", 853, 480);
      TAG(("p"), TEXT("Så er andet afsnit af topDatamat her endeligt!"));
      TAG(("p"),
        TEXT("Nyd det her eller ")
        TAG(("a", "href", "https://www.youtube.com/watch?v=F3HHS50dA6g"), TEXT("direkte på YouTube"))
        TEXT("."))))

  webpage_end(builder);
  print_tree(builder->top_node, 0);

  /*  test();*/

  return 0;
}
