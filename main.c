#include "parser.h"

int main() {
  fprintf(stderr, "repl> ");
  get_next_token();
  main_loop();
  return 0;
}
