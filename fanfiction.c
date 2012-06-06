#include "thc.h"

#include <stdio.h>

int pagemain(int argc, char** argv) {
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  UNUSED(argc);
  UNUSED(argv);
  
  webpage_start(builder, "FanFiction", "Fanfiction");

  TAG(("article"),
    TAG(("header"),
      TAG(("h1"), TEXT("Fanfiction"));
    TAG(("p"), TEXT("Flere har klaget til indbakken at der ikke er et sted hvor man dele sine egne visioner og idéer til topDatamat."));
    TAG(("p"), TEXT("For at rette op på denne mangel, har vi skabt denne underside."));
    TAG(("p"), TEXT("Men her er først lidt inspiration:"));
    TAG(("form", "method", "post", "enctype", "multipart/form-data"),
      TAG(("fieldset"),
        TAG(("legend"), TEXT("Indsend fanfiction"));
        TAG(("p"), TEXT("Er vi vilde med dit fanfiction, så kan det være vi putter det på hjemmeside, så alle kan læse det."));
        TAG(("label", "for", "fanfiction-title"), TEXT("Titel (påkrævet):"));
        TAG(("input", "type", "text", "id", "fanfiction-title", "name", "fanfiction-title"), );
        TAG(("label", "for", "fanfiction-author"), TEXT("Forfatter (påkrævet):"));
        TAG(("input", "type", "text", "id", "fanfiction-author", "name", "fanfiction-author"), );
        TAG(("label", "for", "fanfiction-email"), TEXT("Elektropost:"));
        TAG(("input", "type", "text", "id", "fanfiction-email", "name", "fanfiction-email"), );
        TAG(("label", "for", "fanfiction-text"), TEXT("Værket (påkrævet):"));
        TAG(("textarea", "cols", "52", "rows", "10", "id", "fanfiction-text", "name", "fanfiction-text"), );
        TAG(("input", "type", "submit", "name", "fanfiction-submit", "value", "Indsend"), );
        )
      )
    )
  )

  webpage_end(builder);

  print_tree(builder->top_node, 0);

  return 0;
}
