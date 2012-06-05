#include "thc.h"

#include <stdio.h>

struct term {
  char *term;
  char *abbr;
  char *prefix;
  struct term *first_translation;
  struct term *next_term;
};

struct dictionary {
  struct term *first_term;
  struct term *last_term;
};

struct term* mkterm(va_list vl) {
  char *val;
  int mode = 0;
  struct term *term = NULL;
  struct term *prev_term = NULL;
  struct term *first_term = NULL;
  struct term *trans = NULL;
  struct term *prev_trans = NULL;
  struct term *first_trans = NULL;
  for ( val=va_arg(vl,char*); val; val=va_arg(vl,char*) ) {
    if ( val == NULL ) {
      mode = 1;
      continue;
    }
    if ( mode == 0 ) {
      if ( first_term == NULL ) {
        first_term = malloc(sizeof(struct term));
        first_term->term = val;
        prev_term = first_term;
      } else {
        term = malloc(sizeof(struct term));
        term->term = val;
        prev_term->next_term = term;
        prev_term = term;
      }
    } else {
      if ( first_trans == NULL ) {
        first_trans = malloc(sizeof(struct term));
        first_trans->term = val;
        prev_trans = first_trans;
      } else {
        trans = malloc(sizeof(struct term));
        trans->term = val;
        prev_trans->next_term = trans;
        prev_trans = trans;
      }
    }
  }
  term = first_term;
  while ( term ) {
    term->first_translation = first_trans;
    term = term->next_term;
  }
  return first_term;
}

void append_term(struct dictionary *dictionary,
                 ...) {
  struct term *new_term;
  struct term *term;
  va_list vl;
  va_start(vl,dictionary);
  new_term = mkterm(vl);
  va_end(vl);
  if ( dictionary->first_term == NULL )
    dictionary->first_term = new_term;
  else {
    dictionary->last_term->next_term = new_term;
  }
  term = dictionary->last_term;
  while ( term ) {
    dictionary->last_term = term;
    term = term->next_term;
  }
}

struct dictionary* create_dictionary() {
  struct dictionary *dictionary;
  dictionary = malloc(sizeof(struct dictionary));
  append_term(dictionary, "Datamat", NULL, "Computer");
  append_term(dictionary, "Mappedatamat", NULL, "Laptop computer");
  append_term(dictionary, "Fastpladelager", NULL, "Harddisk");
  append_term(dictionary, "Oversætter", NULL, "Compiler");
  append_term(dictionary, "Central beregneenhed", "Central bregningsenhed", 
    NULL, "Central Processing Unit", "CPU");
  /*
		array ('Tavledatamat', 'Tablet'),
		array ('Datafon', 'Smartphone'),
		array ('Almen Seriel Bus|asb', array('Universal Serial Bus', 'USB')),
		array ('Arbejdslager', array('Random Access Memory', 'RAM')),
//		array ('Fastlagermassiv', array('Solid State Disk', 'SSD')),
		array ('Tastatur', 'Keyboard'),
		array ('Flugttast', 'Escape key'),
		array ('Vognretur', 'Carriage return'),
		array (array('Skiftetast', 'Skiftenøgle'), 'Shift key'),
		array ('Navnerum', 'Namespace'),
		array ('Virkefelt', 'Scope'),
		array ('Verdensomspændende Spindel,Det', array ('World Wide Web', 'WWW', 'Web')),
		array (array('Elektropost', 'E-brev'), 'E-mail'),
		array ('Spindellæser', 'Web browser'),
		array ('Løsen', 'Password'),
		array ('Systemskal', array ('Command shell', 'Shell')),
		array ('Elektronisk Databehandling|edb', array ('Information Technology', 'IT')),
		array ('Skalprogram', 'Shell script'),
		array ('Brandmur', 'Firewall'),
		array ('Udgave', 'Version'),
		array ('Sikkerhedskopi', 'Backup'),
		array ('Linjeskriver', 'Printer'),
//		array ('Stødpude', 'Buffer'),
		array (array('Tekstbehandlingsprogram', 'Tekstredigeringsværktøj'), array('Text editor', 'Editor')),
		array ('Kildetekst', 'Source code'),
		array ('Råtekst', 'Plain text'),
		array ('Prikkode', array('Quick Response Code', 'QR Code')),
		array ('Topdatamat', 'Supercomputer'),
		array ('Programmel', 'Software'),
		array ('Maskinel', 'Hardware'),
		array ('Kontaktskærm', 'Touch screen'),
		array ('Borddatamat', 'Desktop computer'),
		array ('Spindelsidefortolker', 'Web browser engine'),
		array ('Datamatspil', array('Video game', 'Computer game')),
		array ('Greltegn', array('Majuscule', 'Capital letter', 'Uppercase letter')),
		array ('Klejntegn', array('Minuscule', 'Lowercase letter')),
		array ('Hjemmedatamat', array('Personal Computer', 'PC')),
		array ('Kodegrube', 'Source repository'),
		array ('Puf', 'Push (changes)'),
		array ('Hal', 'Pull (changes)'),
		array (array('Tving', 'Gennemtvinge'), 'Force (changes)'),
		array (array('Sammenflet(ning)', 'Flet'), 'Merge'),
		array ('Lus', 'Bug'),
		array ('Aflusning', 'Debugging'),
		array ('Afluser', 'Debugger'),
		array ('Skyen', 'The cloud'),
		array ('Hjemmeside', 'Web site'),
  */
  return dictionary;
}

int pagemain(int argc, char** argv) {
  struct html_builder builder;
  struct dictionary *dictionary;
  struct term *term;
  /*char *tmp;*/
  UNUSED(argc);
  UNUSED(argv);
  
  webpage_start(&builder, "Dictionary", "Ordbog");
  
  dictionary = create_dictionary();
  
  enter_tag(&builder, "article", NULL);
  enter_tag(&builder, "header", NULL);
  enter_tag(&builder, "h1", NULL);
  insert_text(&builder, "Ordbog");
  leave_tag(&builder); /* h1 */
  leave_tag(&builder); /* header */
  
  term = dictionary->first_term;
  
  enter_tag(&builder, "dl", "class", "dictionary", NULL);
  while ( term ) {
    enter_tag(&builder, "dt", NULL);
    insert_text(&builder, term->term);
    leave_tag(&builder); /* dt */
    enter_tag(&builder, "dd", NULL);
    insert_text(&builder, term->first_translation->term);
    leave_tag(&builder); /* dd */
    term = term->next_term;
  }
  leave_tag(&builder); /* dl.dictionary */
  leave_tag(&builder); /* article */
  
  webpage_end(&builder);

  print_tree(builder.top_node, 0);

  return 0;
}
