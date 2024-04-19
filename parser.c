#include "parser.h"

TokenData current_token;

ast_node *get_lhs() { return variable_expr_ast_create("x"); }
ast_node *get_rhs() { return variable_expr_ast_create("y"); }
ast_node *get_result() {
  return binary_expr_ast_create(ADD, get_lhs(), get_rhs());
}

int get_next_token() {
  current_token = get_token();
  return current_token.this_char;
}

void log_error(const char *value) { fprintf(stderr, "Error: %s\n", value); }
void log_errorp(const char *value) { log_error(value); }
ast_node *parse_number_expr() {
  number_expr_ast_create(current_token.num_val);
  return NULL;
}
