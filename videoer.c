#include "thc.h"

#include <stdio.h>

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

void append_episode(struct season *current_season,
                    int number,
                    char *da_id, char *da_title,
                    char *en_id, char *en_title) {
  struct episode *new_episode;
  new_episode = malloc(sizeof(struct episode));
  new_episode->number = number;
  new_episode->da.watchid = da_id;
  new_episode->da.title = da_title;
  if ( en_id != NULL ) {
    new_episode->en.watchid = en_id;
    new_episode->en.title = en_title;
  } else {
    new_episode->en.watchid = NULL;
    new_episode->en.title = NULL;
  }
  if ( current_season->first_episode == NULL )
    current_season->first_episode = new_episode;
  else
    current_season->last_episode->next_episode = new_episode;
  current_season->last_episode = new_episode;
}

struct season* videos_init() {
  struct season *first_season;
  struct season *current_season;
  first_season = create_season(NULL, 0);
  current_season = first_season;
  append_episode(current_season, 0,
                 "lgmA9z8Sb5E", NULL,
                 "0ohC89vJjnY", NULL);
  append_episode(current_season, 1,
                 "F3HHS50dA6g", "DIKUrevy-afsnit",
                 NULL, NULL);
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
                      TAG(("header"), TAG(("h1"), TEXT("Med danske undertekster")));
                      webpage_youtubeembed(builder, episode->da.watchid, 480, 274);
                      TAG(("p"),
                          TAG(("a", "href", youtube_url(episode->da.watchid)),
                              TEXT("Se på YouTube"))))
                  if ( episode->en.watchid != NULL ) {
                    TAG(("article", "class", "episode-en"),
                        TAG(("header"),
                            TAG(("h1"), TEXT("With English subtitles")))
                        webpage_youtubeembed(builder, episode->en.watchid, 480, 274);
                        TAG(("p"),
                            TAG(("a", "href", youtube_url(episode->en.watchid), NULL),
                                TEXT("Watch on YouTube"))))
                      })
                episode = episode->next_episode;
            })
          season = season->next_season;
      });
  webpage_end(builder);

  print_tree(builder->top_node, 0);

  return 0;
}
