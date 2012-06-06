#include "thc.h"

#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int cmp_term(const void *x, const void *y) {
   char *termx = ((const struct term*)x)->term;
  char *termy = ((const struct term*)y)->term;
  if (strchr(termx, '|') != NULL) {
    termx = strchr(termx, '|')+1;
  }
  if (strchr(termy, '|') != NULL) {
    termy = strchr(termy, '|')+1;
  }
  return strcmp(termx, termy);
}

char *blank_char(char *str, char chr) {
  char *p;
  p = str;
  while ( (p = strchr(p, chr)) != NULL ) {
    *p = ' ';
  }
  return p;
}

int pagemain(int argc, char** argv) {
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  char *term;
  struct dictionary *dict;
  UNUSED(argc);
  UNUSED(argv);

  webpage_start(builder, "Dictionary", "Ordbog");

  dict = read_dictionary("ordbog.data");

  TAG(("article"),
      TAG(("header"), TAG(("h1"), TEXT("Ordbog")))

      if (dict == NULL) {
        TAG(("p"), TEXT("Cannot read dictionary."))
      } else {
        qsort(dict->terms, dict->num_terms, sizeof(struct term), cmp_term);
        TAG(("dl", "class", "dictionary"),
            for (size_t i = 0; i < dict->num_terms; i++) {
              TAG(("dt"),
                  term = dict->terms[i].term;
                  if (strchr(term, '|') != NULL) {
                    term = blank_char(term, '|');
                  }
                  TEXT(dict->terms[i].term);
                  if (dict->terms[i].abbr) {
                    TEXT("(",dict->terms[i].abbr,")");
                  })
                TAG(("dd"), TEXT(dict->terms[i].translation));
            });
      });
  webpage_end(builder);

  print_tree(builder->top_node, 0);

  return 0;
}
