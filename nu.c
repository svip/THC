#include "thc.h"

#include <stdio.h>

const char *source_file = __FILE__;

int pagemain(int argc, char** argv) {
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  UNUSED(argc);
  UNUSED(argv);

  webpage_start(builder, "Now", "Nu");

  TAG(("article"),
      TAG(("header"),
          TAG(("h1"), TEXT("Hvor er de nu?");))
      TAG(("article"),
          TAG(("p"),
              TEXT("Meget er sket siden topDatamat udkom i 2011. Men hvad er der egentlig blevet af værterne siden vi forlod dem i kælderen under DIKU? Det vil vi her prøve at kaste lys over."))
          TAG(("h2"), TEXT("Truls"))
          TAG(("p"),
               TEXT("Efter mange år kunne Truls ikke længere udsætte sit biologiske ur, og han forlod kælderen fulgte sine instinkter tilbage til Norge hvor han tog sig en PhD i Trondhjem."))
          TAG(("h2"), TEXT("Brainfuck"))
          TAG(("p"),
               TEXT("Den driftige Brainfuck forlod kælderen og gjorde sin karriere i industrien, og er i dag fabriksejer i en mindre vestsjællandsk stationsby."))
          TAG(("h2"), TEXT("Troels"))
          TAG(("p"),
               TEXT("Troels forlod kælderen og gik ca. 150 meter og sidder nu i et kontor på HCØ."))))

  webpage_end(builder);

  print_tree(builder->top_node, 0);

  return 0;
}
