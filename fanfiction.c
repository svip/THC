#include "thc.h"

#include <stdio.h>
#include <stdlib.h>

#define DATA_DIR "/var/www/topdatamat/c/data"
#define MAXLEN 5000
/* For adding line breaking and trailing NUL */
#define MAXINPUT MAXLEN+2

void uudecode(char *src, char *last, char *dest) {
  for (; src != last; src++, dest++) {
    if (*src == '+')
      *dest = ' ';
    else if (*src == '%') {
      unsigned int code;
      if ( sscanf(src+1, "%2x", &code) != 1 ) code = '?';
      *dest = code;
      src += 2;
    } else
      *dest = *src;
  }
  *dest = '\n';
  *++dest = '\0';
}

int handle_postdata ( void ) {
  char *lenstr, *dump;
  char input[MAXINPUT], data[MAXINPUT];
  long len;
  const char *datadir = getenv("DATA_DIR") ? getenv("DATA_DIR") : DATA_DIR;
  
  lenstr = getenv("CONTENT_LENGTH");
  if ( lenstr == NULL
    || (len = atoi(lenstr)) <= 1
    || len > MAXLEN )
    return 1;
  
  FILE *f;
  dump = fgets(input, len+1, stdin);
  if ( dump == NULL )
    return 1;
  
  uudecode(input, input+len, data);
  
  char path[75];
  sprintf(path, "%s/%s", datadir, "test.dump");
  f = fopen(path, "a");

  if ( f == NULL )
    return 1;

  fputs(data, f);
  fclose(f);
  
  return 0;
}

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
    if ( getenv("CONTENT_LENGTH") != NULL ) {
      if ( handle_postdata() == 0 ) {
    TAG(("p", "style", "font-weight: bold;"), TEXT("Tak for din fanfiction, vi vil græde (af glæde) over dets indhold!"));
      } else {
    TAG(("p", "style", "font-weight: bold;"), TEXT("Dit inddata gik i stykker, hvad fanden har du gang i?!"));
      }
    } else {
    TAG(("form", "method", "post"),/*, "enctype", "multipart/form-data"),*/
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
        TAG(("textarea", "cols", "52", "rows", "10", "id", "fanfiction-text", "name", "fanfiction-text"), TEXT(""));
        TAG(("input", "type", "submit", "name", "fanfiction-submit", "value", "Indsend"), );
        )
      )
    }
    )
  )

  webpage_end(builder);

  print_tree(builder->top_node, 0);

  return 0;
}
