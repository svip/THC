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
  { "Det |Verdensomspændende Spindel", NULL, "World Wide Web, WWW eller Web" },
  { "Aflusning", NULL, "Debugging" },
  { "Kodegrube", NULL, "Source repository" },
  { "Mappedatamat", NULL, "Laptop" },
};
static const int dictionary_entries = sizeof(dictionary)/sizeof(struct term);

int cmp_term(const void *x, const void *y) {
  int cmp;
  char *termx, *termy;
  termx = ((const struct term*)x)->term;
  if (strchr(termx, '|') != NULL) {
    termx = strchr(termx, '|');
  }
  termy = ((const struct term*)y)->term;
  if (strchr(termy, '|') != NULL) {
    termy = strchr(termy, '|');
  }
  cmp = strcmp(termx, termy);
  return cmp;
}

char *remove_char(char *str, char chr) {
  char *p;
  p = str;
  while ( (p = strchr(p, chr)) != NULL ) {
    memmove(p, p+1, strlen(p));
  }
  return p;
}

int pagemain(int argc, char** argv) {
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  char *term;
  UNUSED(argc);
  UNUSED(argv);

  qsort(dictionary, dictionary_entries, sizeof(struct term), cmp_term);

  webpage_start(builder, "Dictionary", "Ordbog");

  TAG(("article"),
      TAG(("header"), TAG(("h1"), TEXT("Ordbog")))
      TAG(("dl", "class", "dictionary"),
          for (int i = 0; i < dictionary_entries; i++) {
            term = dictionary[i].term;
            if (strchr(term, '|') != NULL)
              term = remove_char(term, '|');
            TAG(("dt"),
                TEXT(term);
                if (dictionary[i].abbr) {
                  TEXT("(",dictionary[i].abbr,")");
                })
              TAG(("dd"), TEXT(dictionary[i].translation));
          }));

  webpage_end(builder);

  print_tree(builder->top_node, 0);

  return 0;
}
