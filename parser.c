#include "parser.h"

int cursor_token;

ast_node *get_lhs() { return variable_expr_ast_create("x"); }
ast_node *get_rhs() { return variable_expr_ast_create("y"); }
ast_node *get_result() {
  return binary_expr_ast_create(ADD, get_lhs(), get_rhs());
}

int get_next_token() {
  cursor_token = get_token();
  return cursor_token;
}
