#include "thc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *source_file = __FILE__;

#define MAXLEN 5000
/* For adding line breaking and trailing NUL */
#define MAXINPUT MAXLEN+2
#define MAXOUTPUT MAXINPUT*2

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

void translate_brainfuck ( char *output, char *code ) {
  output = "";
  
  sprintbf(output, "#include <stdio.h>\n#include <stdlib.h>\n\nint main (int argc, char** argv) {\nUNUSED(argc);\nUNUSED(argv);\nunsigned char* ptr;\n\n");
  
  for (int codep = 0; code[codep] != 0; codep++) {
    switch(code[codep]) {
    case '+':
      sprintbf(output, "(*ptr)++;\n");
      break;
    case '-':
      sprintbf(output, "(*ptr)--;\n");
      break;
    case '>':
      sprintbf(output, "ptr++;\n");
      break;
    case '<':
      sprintbf(output, "ptr--;\n");
      break;
    case '.':
      sprintbf(output, "putchar(*ptr);\n");
      break;
    case ',':
      sprintbf(output, "*ptr=getchar();\n");
      break;
    case '[':
      sprintbf(output, "while (*ptr) {\n");
      break;
    case ']':
      sprintbf(output, "}\n");
      break;
    }
  }
  
  sprintbf(output, "return 0;\n}\n");
}

int handle_postdata ( char *output ) {
  char *lenstr, *dump;
  char input[MAXINPUT], data[MAXINPUT];
  long len;
  
  lenstr = getenv("CONTENT_LENGTH");
  if ( lenstr == NULL
    || (len = atoi(lenstr)) <= 1
    || len > MAXLEN )
    return 1;
  
  dump = fgets(input, len+1, stdin);
  if ( dump == NULL )
    return 1;
  
  uudecode(input, input+len, data);
  
  translate_brainfuck(output, data);
  
  return 0;
}

int pagemain(int argc, char** argv) {
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  UNUSED(argc);
  UNUSED(argv);
  char output[MAXOUTPUT] = { 0 };
  
  webpage_start(builder, "Brainfuck", "Brainfuck-fortolker");

  TAG(("article"),
    TAG(("header"),
      TAG(("h1"), TEXT("Brainfuck-fortolker"));
    TAG(("p"), TEXT("Har du også svært ved at programmere i C, men kan sagtens finde ud af at programmere i Brianfuck?"));
    TAG(("p"), TEXT("Så er denne fortolker noget for dig!"));
    if ( getenv("CONTENT_LENGTH") != NULL ) {
      if ( handle_postdata(output) == 0 ) {
    TAG(("p", "style", "font-weight: bold;"), TEXT("Her er dit nye C-program:"));
    TAG(("pre"), TEXT(output));
      } else {
    TAG(("p", "style", "font-weight: bold;"), TEXT("Hva sulan!  Dit Brainfuck-kode kunne ikke oversættes til C!"));
      }
    } else {
    TAG(("form", "method", "post"),/*, "enctype", "multipart/form-data"),*/
      TAG(("fieldset"),
        TAG(("legend"), TEXT("Brainfuck til C"));
        TAG(("label", "for", "brainfuck-program"), TEXT("Programmet:"));
        TAG(("textarea", "type", "text", "id", "brainfuck-program", "name", "brainfuck-program"), TEXT(""));
        TAG(("input", "type", "submit", "name", "brainfuck-submit", "value", "Oversæt!"), );
        )
      )
    }
    )
  )

  webpage_end(builder);

  print_tree(builder->top_node, 0);

  return 0;
}
