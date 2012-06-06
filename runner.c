#include "thc.h"

#include <unistd.h>
#include <signal.h>
#include <time.h>

void signal_insertrow(struct html_builder *builder,
                      char *signal_name,
                      char *signal_code,
                      char *value) {
  TAG(("tr"),
      TAG(("td"),
          TEXT(signal_name);
          TAG(("tt"), TEXT(signal_code)))
      TAG(("td"), TEXT(value)))
    }

void signal_handler(int signal, siginfo_t *info, void *context) {
  char *error, *tmp = malloc(64);
  struct html_builder builderv;
  struct html_builder *builder = &builderv;
  UNUSED(info);
  UNUSED(context);
  switch (signal) {
  case SIGSEGV:
    error = "Lagersegmentfejl";
    break;
  case SIGBUS:
    error = "Busfejl";
    break;
  default:
    error = "Løbsk undtagelse";
  }
  webpage_start(builder, NULL, "Fejl");
  TAG(("article"),
      TAG(("header"),
          TAG(("h1"),
              sprintf(tmp, "Fejl: %s", error);
              TEXT(tmp)));
      TAG(("table", "class", "error-table"),
          TAG(("thead"),
              TAG(("tr"),
                  TAG(("th"), TEXT("Variabel"));
                  TAG(("th"), TEXT("Værdi"))));
          TAG(("tbody"),
              tmp = malloc(32); sprintf(tmp, "%d", info->si_signo);
              signal_insertrow(builder, "Signal number", "si_signo", tmp);
              tmp = malloc(32); sprintf(tmp, "%d", info->si_errno);
              signal_insertrow(builder, "Error number", "si_errno", tmp);
              tmp = malloc(32); sprintf(tmp, "%d", info->si_code);
              signal_insertrow(builder, "Signal code", "si_code", tmp);
              /*
                tmp = malloc(32); sprintf(tmp, "%d", info->si_trapno);
                signal_insertrow(builder, "Trap number", "si_trapno", tmp);
              */
              tmp = malloc(32); sprintf(tmp, "%d", info->si_pid);
              signal_insertrow(builder, "Process id", "si_pid", tmp);
              tmp = malloc(32); sprintf(tmp, "%d", info->si_uid);
              signal_insertrow(builder, "User id", "si_uid", tmp);
              tmp = malloc(32); sprintf(tmp, "%d", info->si_status);
              signal_insertrow(builder, "Exit status", "si_status", tmp);
              tmp = malloc(32); sprintf(tmp, "%f", ((float)info->si_utime/CLOCKS_PER_SEC));
              signal_insertrow(builder, "User time", "si_utime", tmp);
              tmp = malloc(32); sprintf(tmp, "%f", ((float)info->si_stime/CLOCKS_PER_SEC));
              signal_insertrow(builder, "System time", "si_stime", tmp);
              /*
                tmp = malloc(32); sprintf(tmp, "%f", (float)info->si_value);
                signal_insertrow(builder, "Signal value", "si_value", tmp);
              */
              tmp = malloc(32); sprintf(tmp, "%d", info->si_int);
              signal_insertrow(builder, "POSIX signal", "si_int", tmp);
              )));
  webpage_end(builder);
  print_tree(builder->top_node, 0);
  exit(1);
}

int main(int argc, char** argv) {
  struct sigaction action;
  action.sa_sigaction = signal_handler;
  action.sa_flags = SA_SIGINFO;
  sigemptyset(&action.sa_mask);
  sigaction(SIGSEGV, &action, NULL);
  printf("Content-type: text/html; charset=utf-8\r\n");
  printf("Cache-control: max-age=3600\r\n");
  printf("\r\n");
  printf("<!doctype html>\r\n");
  return pagemain(argc, argv);
}
