#include "parser.h"
#include <stdio.h>

int main() {
  fprintf(stderr, "repl>\n");
  get_next_token();
  main_loop();
  return 0;
}