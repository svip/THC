#include "thc.h"

#include <stdio.h>

const char *source_file = __FILE__;

int pagemain(int argc, char** argv) {
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  UNUSED(argc);
  UNUSED(argv);
  
  webpage_start(builder, "Front", NULL);

  TAG(("article"),
    TAG(("header"),
      TAG(("h1"),
        TAG(("b"), TEXT("topDatamat 0x2"));
        TEXT(" (del 0/1) udgivet! (");
        TAG(("a", "href", "http://youtube.com/watch?v=KI-XrxabmhM"),
          TEXT("Click here for English subtitles"))
        TEXT(")")));
    TAG(("div", "class", "post"),
      webpage_youtubeembed(builder, "a0v4xMElWS8", 853, 480);
      TAG(("p"), TEXT("Så er første del af GOTO-konference-afsnitte af topDatamat her endeligt!"));
      TAG(("p"),
        TEXT("Nyd det her eller ")
        TAG(("a", "href", "https://www.youtube.com/watch?v=a0v4xMElWS8"), TEXT("direkte på YouTube"))
        TEXT("."))));

  webpage_end(builder);
  print_tree(builder->top_node, 0);

  /*  test();*/

  return 0;
}
