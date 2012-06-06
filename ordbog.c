#include "thc.h"

#include <stdio.h>

char* website_pagename = "ordbog";

struct term {
  char *term;
  char *abbr;
  char *translation;
};

struct term dictionary[] = {
  { "Afluser", NULL, "Debugger" },
  { "Aflusning", NULL, "Debugging" },
  { "Almen Seriel Bus", "asb", "Universal Serial Bus eller USB" },
  { "Arbejdslager", NULL, "Random Access Memory eller RAM" },
  { "Borddatamat", NULL, "Desktop computer" },
  { "Brandmur", NULL, "Firewall" },
  { "Central beregneenhed", NULL, "Central Processing Unit eller CPU"},
  { "Central beregningsenhed", NULL, "Central Processing Unit eller CPU"},
  { "Datafon", NULL, "Smartphone" },
  { "Datamat", NULL, "Computer"},
  { "Datamatspil", NULL, "Video game eller Computer game"},
  { "E-brev", NULL, "E-mail"},
  { "Elektronisk Databehandling (edb)", NULL, "Information Technology eller IT" },
  { "Elektropost", NULL, "E-mail" },
  { "Fastpladelager", NULL, "Harddisk" },
  { "Flet", NULL, "Merge" },
  { "Flugttast", NULL, "Escape key" },
  { "Gennemtvinge", NULL, "Force (changes)" },
  { "Greltegn", NULL, "Majuscule, Capital letter eller Uppercase letter" },
  { "Hal", NULL, "Pull (changes)" },
  { "Hjemmedatamat", NULL, "Personal Computer eller PC" },
  { "Hjemmeside", NULL, "Web site" },
  { "Kildetekst", NULL, "Source code" },
  { "Klejntegn", NULL, "Minuscule eller Lowercase letter" },
  { "Kodegrube", NULL, "Source repository" },
  { "Kontaktskærm", NULL, "Touch screen" },
  { "Linjeskriver", NULL, "Printer" },
  { "Lus", NULL, "Bug" },
  { "Løsen", NULL, "Password" },
  { "Mappedatamat", NULL, "Laptop" },
  { "Maskinel", NULL, "Hardware" },
  { "Navnerum", NULL, "Namespace" },
  { "Oversætter", NULL, "Compiler"},
  { "Prikkode", NULL, "Quick Response Code eller QR Code" },
  { "Programmel", NULL, "Software" },
  { "Puf", NULL, "Push (changes)" },
  { "Råtekst", NULL, "Plain text"},
  { "Sammenflet(ning)", NULL, "Merge"},
  { "Sikkerhedskopi", NULL, "Backup" },
  { "Skalprogram", NULL, "Shell script" },
  { "Skiftenøgle", NULL, "Shift key" },
  { "Skiftetast", NULL, "Shift key" },
  { "Skyen", NULL, "The cloud" },
  { "Spindellæser", NULL, "Web browser" },
  { "Spindelsidefortolker", NULL, "Web browser engine" },
  { "Systemskal", NULL, "Command shell eller Shell" },
  { "Tastatur", NULL, "Keyboard" },
  { "Tavledatamat", NULL, "Tablet" },
  { "Tekstbehandlingsprogram", NULL, "Text editor eller Editor" },
  { "Tekstredigeringsværktøj", NULL, "Text editor eller Editor" },
  { "Topdatamat", NULL, "Supercomputer" },
  { "Tving", NULL, "Force (changes)" },
  { "Udgave", NULL, "Version" },
  { "Det Verdensomspændende Spindel", NULL, "World Wide Web, WWW eller Web" },
  { "Virkefelt", NULL, "Scope" },
  { "Vognretur", NULL, "Carriage return" }
};
static const int dictionary_entries = sizeof(dictionary)/sizeof(struct term);

int pagemain(int argc, char** argv) {
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  UNUSED(argc);
  UNUSED(argv);

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
