#include "thc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *source_file = __FILE__;

#define DATA_DIR "/var/www/topdatamat/c/data"
#define MAXLEN 5000
/* For adding line breaking and trailing NUL */
#define MAXINPUT MAXLEN+2

/* Copied from http://www.daniweb.com/software-development/c/code/216517/strings-search-and-replace */
char *replace ( const char *src, const char *from, const char *to ) {
  size_t size = strlen(src) + 1;
  size_t fromlen = strlen(from);
  size_t tolen = strlen(to);
  char *value = malloc(size);
  char *dst = value;
  if ( value != NULL ) {
    for ( ;; ) {
      const char *match = strstr(src, from);
      if ( match != NULL )
        {
        size_t count = match - src;
        char *temp;
        size += tolen - fromlen;
        temp = realloc(value, size);
        if ( temp == NULL ) {
          free(value);
          return NULL;
        }
        dst = temp + (dst - value);
        value = temp;
        memmove(dst, src, count);
        src += count;
        dst += count;
        memmove(dst, to, tolen);
        src += fromlen;
        dst += tolen;
      } else {
        /* No match */
        strcpy(dst, src);
        break;
      }
    }
  }
  return value;
}

void uudecode ( char *src, char *last, char *dest ) {
  for (; src < last && *src; src++, dest++) {
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

void write_fanfiction ( char *data, FILE *f ) {
  char *newdata;
  if ( f == NULL )
    return;
  
  newdata = replace(data, "fanfiction-title=", "\\title{");
  newdata = replace(newdata, "&fanfiction-author=", "}\n\\author{");
  newdata = replace(newdata, "&fanfiction-email=", " (");
  newdata = replace(newdata, "&fanfiction-text=", ")}\n\n\\begin{document}\n");
  printbf(newdata, "\\documentclass[a4paper,11pt]{article}\n\\usepackage[utf8]{inputenc}\n\\usepackage[T1]{fontenc}\n\\usepackage[danish]{babel}\n\n%s\n\\end{document}", newdata);
  fputs(newdata, f);
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
  sprintf(path, "%s/%s", datadir, "test.tex");
  f = fopen(path, "a");
  
  if ( f == NULL )
    return 1;
  
  write_fanfiction(data, f);
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
    /*TAG(("p"), TEXT("Men her er først lidt inspiration:"));*/
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
