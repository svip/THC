#include "boilerplate.h"
#include "thc.h"

#include <stdio.h>

int main(int argc, char** argv) {
  printf("HTTP/1.1 200 OK\r\n");
  printf("Server: THC %s\r\n", THC_VERSION);
  printf("Content-type: text/html\r\n");
  printf("Cache-control: max-age=3600\r\n");
  printf("\r\n");
  return pagemain(argc, argv);
}
