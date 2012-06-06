#include "thc.h"

#include <stdio.h>
#include <string.h>

char* website_pagename = "ordbog";

struct term {
  char *term;
  char *abbr;
  char *translation;
};

struct term dictionary[] = {
  { "Hal", NULL, "Pull (changes)" },
  { "Puf", NULL, "Push (changes)" },
  { "Virkefelt", NULL, "Scope" },
  { "Almen Seriel Bus", "asb", "Universal Serial Bus eller USB" },
  { "Flet", NULL, "Merge" },
  { "Tavledatamat", NULL, "Tablet" },
  { "Gennemtvinge", NULL, "Force (changes)" },
  { "Tekstbehandlingsprogram", NULL, "Text editor eller Editor" },
  { "Afluser", NULL, "Debugger" },
  { "Skiftenøgle", NULL, "Shift key" },
  { "Elektronisk Databehandling (edb)", NULL, "Information Technology eller IT" },
  { "Løsen", NULL, "Password" },
  { "Spindelsidefortolker", NULL, "Web browser engine" },
  { "Central beregneenhed", NULL, "Central Processing Unit eller CPU"},
  { "Borddatamat", NULL, "Desktop computer" },
  { "Fastpladelager", NULL, "Harddisk" },
  { "Vognretur", NULL, "Carriage return" },
  { "Oversætter", NULL, "Compiler"},
  { "Maskinel", NULL, "Hardware" },
  { "Skiftetast", NULL, "Shift key" },
  { "Datafon", NULL, "Smartphone" },
  { "Prikkode", NULL, "Quick Response Code eller QR Code" },
  { "Skalprogram", NULL, "Shell script" },
  { "Spindellæser", NULL, "Web browser" },
  { "Skyen", NULL, "The cloud" },
  { "Sammenflet(ning)", NULL, "Merge"},
  { "E-brev", NULL, "E-mail"},
  { "Råtekst", NULL, "Plain text"},
  { "Hjemmedatamat", NULL, "Personal Computer eller PC" },
  { "Arbejdslager", NULL, "Random Access Memory eller RAM" },
  { "Central beregningsenhed", NULL, "Central Processing Unit eller CPU"},
  { "Linjeskriver", NULL, "Printer" },
  { "Kildetekst", NULL, "Source code" },
  { "Datamatspil", NULL, "Video game eller Computer game"},
  { "Flugttast", NULL, "Escape key" },
  { "Systemskal", NULL, "Command shell eller Shell" },
  { "Tekstredigeringsværktøj", NULL, "Text editor eller Editor" },
  { "Datamat", NULL, "Computer"},
  { "Kontaktskærm", NULL, "Touch screen" },
  { "Sikkerhedskopi", NULL, "Backup" },
  { "Elektropost", NULL, "E-mail" },
  { "Lus", NULL, "Bug" },
  { "Programmel", NULL, "Software" },
  { "Topdatamat", NULL, "Supercomputer" },
  { "Navnerum", NULL, "Namespace" },
  { "Tastatur", NULL, "Keyboard" },
  { "Klejntegn", NULL, "Minuscule eller Lowercase letter" },
  { "Hjemmeside", NULL, "Web site" },
  { "Greltegn", NULL, "Majuscule, Capital letter eller Uppercase letter" },
  { "Udgave", NULL, "Version" },
  { "Brandmur", NULL, "Firewall" },
  { "Tving", NULL, "Force (changes)" },
  { "Verdensomspændende Spindel,Det", NULL, "World Wide Web, WWW eller Web" },
  { "Aflusning", NULL, "Debugging" },
  { "Kodegrube", NULL, "Source repository" },
  { "Mappedatamat", NULL, "Laptop" },
};
static const int dictionary_entries = sizeof(dictionary)/sizeof(struct term);

int cmp_term(const void *x, const void *y) {
  char *suffix = malloc(6);
  char *full = malloc(64);
  char *term = malloc(64);
  term = ((const struct term*)x)->term;
  if (strchr(term, ',') != NULL) {
    strncpy(suffix, strchr(term, ','), strlen(term));
    strncpy(full, term, strlen(term)-strlen(suffix));
    sprintf(((const struct term*)x)->term, "%s %s", full, suffix);
  }
  term = malloc(64);
  term = ((const struct term*)y)->term;
  if (strchr(term, ',') != NULL) {
    strncpy(suffix, strchr(term, ','), strlen(term));
    strncpy(full, term, strlen(term)-strlen(suffix));
    sprintf(((const struct term*)y)->term, "%s %s", full, suffix);
  }
  return strcmp(((const struct term*)x)->term, ((const struct term*)y)->term);
}

int pagemain(int argc, char** argv) {
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  UNUSED(argc);
  UNUSED(argv);

  qsort(dictionary, dictionary_entries, sizeof(struct term), cmp_term);

  webpage_start(builder, "Dictionary", "Ordbog");

  TAG(("article"),
      TAG(("header"), TAG(("h1"), TEXT("Ordbog")))
      TAG(("dl", "class", "dictionary"),
          for (int i = 0; i < dictionary_entries; i++) {
            TAG(("dt"),
                TEXT(dictionary[i].term);
                if (dictionary[i].abbr) {
                  TEXT("(",dictionary[i].abbr,")");
                })
              TAG(("dd"), TEXT(dictionary[i].translation));
          }));

  webpage_end(builder);

  print_tree(builder->top_node, 0);

  return 0;
}
