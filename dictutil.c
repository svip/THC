#include "thc.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *source_file = NULL; /* Gross hack */

char *progname;

void usage() {
  fprintf(stderr, "Usage: %s <file> <insert|delete> args...\n", progname);
  exit(1);
}

struct dictionary *dict_or_death(const char *path) {
  struct dictionary *dict = read_dictionary(path);
  if (dict == NULL) {
    fprintf(stderr, "%s: %s: %s\n", progname, path, strerror(errno));
    exit(2);
  }
  return dict;
}

int main(int argc, char** argv) {
  progname = argv[0];
  if (argc < 3) {
    usage();
  }
  char *file = argv[1];
  if (strcmp(argv[2], "insert") == 0) {
    if (argc < 5 || argc > 6) {
      fprintf(stderr, "Usage: %s %s insert <term> <translation> [abbreviation]\n",
              progname, argv[2]);
      exit(1);
    }
    char *term = argv[3];
    char *translation = argv[4];
    char *abbr = argc == 5 ? argv[5] : NULL;
    struct dictionary *dict = dict_or_death(file);
    struct term *terms = malloc((dict->num_terms+1)*sizeof(struct term));
    memcpy(terms,dict->terms,dict->num_terms*sizeof(struct term));
    terms[dict->num_terms].term = term;
    terms[dict->num_terms].translation = translation;
    terms[dict->num_terms].abbr = abbr;
    dict->terms = terms;
    dict->num_terms++;
    if (unlink(file) == -1) {
      fprintf(stderr, "%s: %s: %s\n", progname, file, strerror(errno));
      exit(2);
    }
    write_dictionary(file, dict);
  } else if (strcmp(argv[2], "delete") == 0) {
    if (argc != 4) {
      fprintf(stderr, "Usage: %s %s delete <term>\n",
              progname, argv[2]);
      exit(1);
    }
    char *term = argv[3];
    struct dictionary *dict = dict_or_death(file);
    size_t pos;

    for (pos = 0; pos < dict->num_terms; pos++) {
      if (strcmp(term,dict->terms[pos].term) == 0) {
        break;
      }
    }

    if (pos == dict->num_terms) {
      fprintf(stderr, "Term %s not found in dictionary.\n", term);
      exit(4);
    }

    memcpy(dict->terms+pos, dict->terms+pos+1, sizeof(struct term)*(dict->num_terms-pos-1));

    dict->num_terms--;
    if (unlink(file) == -1) {
      fprintf(stderr, "%s: %s: %s\n", progname, file, strerror(errno));
      exit(2);
    }
    write_dictionary(file, dict);
  } else {
    fprintf(stderr, "Unknown command: %s\n", file);
    usage();
  }
}
