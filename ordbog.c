#include "thc.h"

#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

const char *source_file = __FILE__;

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

struct dictionary* gross_ugly_hack(const char *path) {
  struct dictionary *ret = NULL;
  struct stat statbuf;
  char *data;
  int fd = open(path, O_RDWR);

  if (fd == -1) {
    return NULL;
  }

  fstat (fd,&statbuf);

  data = mmap(NULL, statbuf.st_size,
              PROT_READ, MAP_SHARED, fd, 0);

  long long base = *(long long*)data;

  ret = malloc(sizeof(*ret));
  ret->this = ret;

  ret->num_terms = ((long long*)data)[1];
  ret->terms = malloc(ret->num_terms * sizeof(struct term));
  for (unsigned int i = 0; i < ret->num_terms; i++) {
    long long term_offset = ((long long*)data)[3+i*3];
    long long abbr_offset = ((long long*)data)[3+i*3+1];
    long long transl_offset = ((long long*)data)[3+i*3+2];
    ret->terms[i].term = data+(term_offset-base);
    if (abbr_offset) {
      ret->terms[i].abbr = data+(abbr_offset-base);
    }
    ret->terms[i].translation = data+(transl_offset-base);
  }
  close(fd);
  return ret;
}

char *parse_translation(char *translation) {
  /**
   * Intend is to make it parse pipes into a natural
   * language list, such as "a|b|c" becoming "a, b eller c"
   * and let's see if it is possible to add a <span> around
   * the words,
   * e.g. "<span>a</span>, <span>b</span> eller <span>c</span>"
   */
  return translation;
}

int pagemain(int argc, char** argv) {
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  char *term;
  struct dictionary *dict;
  UNUSED(argc);
  UNUSED(argv);

  webpage_start(builder, "Dictionary", "Engelsk ordbog");

  if (sizeof(void*) == 8) {
    dict = read_dictionary("ordbog.data");
  } else {
    /* Oh-uh... we're not a 64-bit machine. */
    dict = gross_ugly_hack("ordbog.data");
  }

  TAG(("article"),
      TAG(("header"), TAG(("h1"), TEXT("Engelsk ordbog")))

      if (dict == NULL) {
        TAG(("p"), TEXT("Cannot read dictionary."))
      } else {
        qsort(dict->terms, dict->num_terms, sizeof(struct term), cmp_term);
        TAG(("dl", "class", "dictionary"),
            for (size_t i = 0; i < dict->num_terms; i++) {
              TAG(("dt"),
                  term = malloc(strlen(dict->terms[i].term));
                  strcpy(term, dict->terms[i].term);
                  if (strchr(term, '|') != NULL) {
                    term = blank_char(term, '|');
                  }
                  TEXT(dict->terms[i].term);
                  if (dict->terms[i].abbr) {
                    TAG(("span", "class", "abbr"),
                      TAG(("abbr", "title", dict->terms[i].term),
                          TEXT(dict->terms[i].abbr))
                    );
                  })
                TAG(("dd"), TEXT(parse_translation(dict->terms[i].translation)));
            });
      });
  webpage_end(builder);

  print_tree(builder->top_node, 0);

  return 0;
}
