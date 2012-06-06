#include "thc.h"

#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char* website_pagename = "ordbog";

struct term {
  char *term;
  char *abbr;
  char *translation;
};

size_t term_size(struct term *term) {
  return strlen(term->term)+1
    + (term->abbr ? strlen(term->abbr)+1 : 0)
    + strlen(term->translation)+1;
}

struct dictionary {
  struct dictionary *this;
  size_t num_terms;
  struct term *terms;
};

int write_dictionary(const char *path, void *start, struct dictionary *dict) {
  int fd = open(path, O_CREAT | O_RDWR | O_TRUNC,  S_IRUSR | S_IWUSR);
  long str_offset = sizeof(*dict) + dict->num_terms * sizeof(struct term);
  long offset = (long)start & ~(sysconf(_SC_PAGE_SIZE) - 1);

  if (-1 == fd) {
    return -1;
  }

  long dict_size = str_offset;

  for (size_t i = 0; i < dict->num_terms; i++) {
    dict_size += term_size(dict->terms+i)+1;
  }

  lseek(fd, dict_size - 1, SEEK_SET);
  write (fd, "", 1);

  void *orig = mmap((void*)offset, dict_size,
                    PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
  char *loc = (char*)orig;
  struct dictionary *newdict = (struct dictionary*)loc;
  struct term *newterms = (struct term*)(loc+sizeof(*newdict));
  newdict->this = newdict;
  newdict->num_terms = dict->num_terms;
  newdict->terms = newterms;
  loc = (char*)newterms;

  for (size_t i = 0; i < dict->num_terms; i++) {
    struct term *term = dict->terms+i;
    strcpy(loc+str_offset,term->term);
    term->term = loc+str_offset;
    str_offset += strlen(term->term)+1;
    if (term->abbr) {
      strcpy(loc+str_offset,term->abbr);
      term->abbr = loc+str_offset;
      str_offset += strlen(term->abbr)+1;
    }
    strcpy(loc+str_offset,term->translation);
    term->translation = loc+str_offset;
    str_offset += strlen(term->translation)+1;
    newterms[i] = *term;
  }

  munmap(orig, dict_size);
  close(fd);
  return 0;
}

struct dictionary* read_dictionary(const char *path) {
  struct dictionary dict;
  int fd = open(path, O_RDWR);

  if (fd == -1) {
    return NULL;
  }

  read(fd, &dict, sizeof(dict));
  dict.this = mmap((void*)dict.this, dict.num_terms,
                   PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0);
  close(fd);
  return dict.this;
}

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
