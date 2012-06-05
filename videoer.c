#include "thc.h"

#include <stdio.h>

struct episode {
  int number;
  union {
    char *watchid;
    char *title;
  } da;
  union {
    char *watchid;
    char *title;
  } en;
  struct episode *next_episode;
};

struct season {
  int number;
  struct episode *first_episode;
  struct episode *last_episode;
  struct season *next_season;
};

/*
char *videos[1][1][2][2] = {
  { / blok 0 *
    { * episode 0 *
      { "lgmA9z8Sb5E", NULL },
      { "0ohC89vJjnY", NULL }
    }
  }
};*/

struct season* create_season(struct season *current_season,
                             int number) {
  struct season *new_season;
  new_season = malloc(sizeof(struct season));
  new_season->number = number;
  if ( current_season == NULL ) {
    return new_season;
  }
  current_season->next_season = new_season;
  return new_season;
}

void append_episode(struct season *current_season,
                    int number,
                    char *da,
                    char *en) {
  struct episode *new_episode;
  new_episode = malloc(sizeof(struct episode));
  new_episode->number = number;
  new_episode->da.watchid = (char*)da[0];
  new_episode->da.title = da[1];
  if ( en != NULL ) {
    new_episode->en.watchid = en[0];
    new_episode->en.title = en[1];
  } else {
    new_episode->en.watchid = NULL;
    new_episode->en.title = NULL;
  }
  if ( current_season->first_episode == NULL )
    current_season->first_episode = new_episode;
  current_season->last_episode = new_episode;
}

int pagemain(int argc, char** argv) {
  struct html_builder builder;
  struct season *first_season;
  struct season *season;
  struct episode *episode;
  char *tmp = malloc(64);
  UNUSED(argc);
  UNUSED(argv);
  
  webpage_start(&builder, "Videos", "Videoer");
  
  first_season = create_season(NULL, 0);
  append_episode(first_season, 0,
                 (char *){ "lgmA9z8Sb5E", NULL },
                 (char *){ "0ohC89vJjnY", NULL });
  
  enter_tag(&builder, "article", NULL);
  enter_tag(&builder, "header", NULL);
  enter_tag(&builder, "h1", NULL);
  insert_text(&builder, "Videoer");
  leave_tag(&builder); /* h1 */
  leave_tag(&builder); /* header */
  season = first_season;
  while ( season ) {
    enter_tag(&builder, "article", "class", "season", NULL);
    enter_tag(&builder, "header", NULL);
    enter_tag(&builder, "h1", NULL);
    sprintf(tmp, "Blok %d", season->number);
    insert_text(&builder, tmp);
    leave_tag(&builder); /* h1 */
    leave_tag(&builder); /* header */
    episode = season->first_episode;
    while ( episode ) {
      enter_tag(&builder, "article", "class", "episode", NULL);
      enter_tag(&builder, "header", NULL);
      enter_tag(&builder, "h1", NULL);
      sprintf(tmp, "%dx%d", season->number, episode->number);
      insert_text(&builder, tmp);
      leave_tag(&builder); /* h1 */
      leave_tag(&builder); /* header */
      enter_tag(&builder, "article", "class", "episode-da", NULL);
      enter_tag(&builder, "header", NULL);
      enter_tag(&builder, "h1", NULL);
      insert_text(&builder, "Med danske undertekster");
      leave_tag(&builder); /* h1 */
      leave_tag(&builder); /* header */
      webpage_youtubeembed(&builder, episode->da.watchid, 480, 274);
      enter_tag(&builder, "p", NULL);
      enter_tag(&builder, "a", "href", youtube_url(episode->da.watchid), NULL);
      insert_text(&builder, "Se på YouTube");
      leave_tag(&builder); /* a */
      leave_tag(&builder); /* p */
      leave_tag(&builder); /* article.episode-da */
      if ( episode->en.watchid != NULL ) {
        enter_tag(&builder, "article", "class", "episode-en", NULL);
        enter_tag(&builder, "header", NULL);
        enter_tag(&builder, "h1", NULL);
        insert_text(&builder, "With English subtitles");
        leave_tag(&builder); /* h1 */
        leave_tag(&builder); /* header */
        webpage_youtubeembed(&builder, episode->en.watchid, 480, 274);
      enter_tag(&builder, "p", NULL);
      enter_tag(&builder, "a", "href", youtube_url(episode->en.watchid), NULL);
      insert_text(&builder, "Watch on YouTube");
      leave_tag(&builder); /* a */
      leave_tag(&builder); /* p */
        leave_tag(&builder); /* article.episode-en */
      }
      leave_tag(&builder); /* article.episode */
      episode = episode->next_episode;
    }
    leave_tag(&builder); /* article.season */
    season = season->next_season;
  }
  
  webpage_end(&builder);

  print_tree(builder.top_node, 0);

  return 0;
}
