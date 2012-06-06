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
        )
      )
    )
  )

  webpage_end(builder);

  print_tree(builder->top_node, 0);

  /*  test();*/

  return 0;
}
