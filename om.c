#include "thc.h"

#include <stdio.h>

int pagemain(int argc, char** argv) {
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  UNUSED(argc);
  UNUSED(argv);

  webpage_start(builder, "About", "Om");

  TAG(("article"),
      TAG(("header"),
          TAG(("h1"), TEXT("Om");)
          TAG(("h6"),
              TEXT("English below")))
      TAG(("article"),
          TAG(("p"),
              TEXT(("Direkte fra DIKUs kælder kommer topDatamat - tre akavede nørder der forsøger at give et retvisende billede af datalogien.  Følg med når Brainfuck udfører dybdeborende interviews, Troels besvarer datalogiens dybeste spørgsmål og den norske Truls forsøger på at integrere sig i en fremmed kultur."))))
      TAG(("header"), TAG(("h1"), TEXT("About")))
      TAG(("p"),
          TEXT("topDatamat is a Danish sketch show with computers and IT in focus.")))

  webpage_end(builder);

  print_tree(builder->top_node, 0);

  return 0;
}
