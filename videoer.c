#include "thc.h"

#include <stdio.h>

const char *source_file = __FILE__;

struct episode {
  int number;
  struct {
    char *watchid;
    char *title;
  } da;
  struct {
    char *watchid;
    char *title;
  } en;
  struct episode *next_episode;
  struct episode *first_special;
  struct episode *last_special;
};

struct season {
  int number;
  struct episode *first_episode;
  struct episode *last_episode;
  struct season *next_season;
};

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

struct episode *append_episode(struct season *current_season,
                    int number,
                    char *da_id, char *da_title,
                    char *en_id, char *en_title) {
  struct episode *new_episode;
  new_episode = malloc(sizeof(struct episode));
  new_episode->number = number;
  new_episode->first_special = NULL;
  new_episode->da.watchid = da_id;
  new_episode->da.title = da_title;
  new_episode->en.watchid = en_id;
  new_episode->en.title = en_title;
  if ( current_season->first_episode == NULL )
    current_season->first_episode = new_episode;
  else
    current_season->last_episode->next_episode = new_episode;
  current_season->last_episode = new_episode;
  return new_episode;
}

void append_specialepisode(struct episode *episode, 
                      char *da_id, char *da_title,
                      char *en_id, char *en_title) {
  struct episode *new_episode;
  new_episode = malloc(sizeof(struct episode));
  new_episode->number = 0;
  new_episode->first_special = NULL;
  new_episode->da.watchid = da_id;
  new_episode->da.title = da_title;
  new_episode->en.watchid = en_id;
  new_episode->en.title = en_title;
  if ( episode->first_special == NULL )
    episode->first_special = new_episode;
  else
    episode->last_special->next_episode = new_episode;
  episode->last_special = new_episode;
}

struct season* videos_init() {
  struct season *first_season;
  struct season *current_season;
  struct episode *working_episode;
  first_season = create_season(NULL, 0);
  current_season = first_season;
  working_episode = append_episode(current_season, 0,
                                 "lgmA9z8Sb5E", NULL,
                                 "0ohC89vJjnY", NULL);
  UNUSED(working_episode);
  working_episode = append_episode(current_season, 1,
                                 "F3HHS50dA6g", "DIKUrevy-afsnit",
                                 "TTVCaZVUvC0", "DIKUrevy episode");
  UNUSED(working_episode);
  /* 
  append_specialepisode(working_episode,
                        "", "Bjarne interview",
                        NULL, NULL);
  */
  return first_season;
}

int pagemain(int argc, char** argv) {
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  struct season *first_season;
  struct season *season;
  struct episode *episode;
  UNUSED(argc);
  UNUSED(argv);
  
  webpage_start(builder, "Videos", "Videoer");
  
  first_season = videos_init();

  TAG(("article"),
      TAG(("header"), TAG(("h1"), TEXT("Videoer")));
      season = first_season;
      while (season) {
        TAG(("article", "class", "season"),
            TAG(("header"),
                TAG(("h1"), TEXT(printbf("Blok %d", season->number))));
            episode = season->first_episode;
            while (episode) {
              TAG(("article", "class", "episode"),
                  TAG(("header"),
                      TAG(("h1"), TEXT(printbf("%dx%d", season->number, episode->number))));
                  TAG(("article", "class", "episode-da"),
                      TAG(("header"),
                      if ( episode->da.title != NULL ) {
                        TAG(("h1"), TEXT(episode->da.title));
                      }
                      TAG(("h2"), TEXT("Med danske undertekster")));
                      webpage_youtubeembed(builder, episode->da.watchid, 480, 274);
                      TAG(("p"),
                          TAG(("a", "href", youtube_url(episode->da.watchid)),
                              TEXT("Se på YouTube"))))
                  if ( episode->en.watchid != NULL ) {
                    TAG(("article", "class", "episode-en"),
                        TAG(("header"),
                        if ( episode->en.title != NULL ) {
                          TAG(("h1"), TEXT(episode->en.title));
                        }
                        TAG(("h2"), TEXT("With English subtitles")))
                        webpage_youtubeembed(builder, episode->en.watchid, 480, 274);
                        TAG(("p"),
                            TAG(("a", "href", youtube_url(episode->en.watchid), NULL),
                                TEXT("Watch on YouTube"))))
                  }
                TAG(("div", "class", "clear"), TEXT("")));
                episode = episode->next_episode;
            })
          season = season->next_season;
      });
  webpage_end(builder);

  print_tree(builder->top_node, 0);

  return 0;
}
