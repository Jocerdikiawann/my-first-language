#include "parser.h"
#include "ast.h"

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
  ast_node *result = number_expr_ast_create(current_token.num_val);
  get_next_token();
  return result;
}

ast_node *parse_paren_expr() {
  get_next_token();
  ast_node *V = parse_expression();
  if (!V) {
    return NULL;
  }
  if (current_token.this_char != ')') {
    log_error("expected ')'");
    return NULL;
  }
  get_next_token();
  return V;
}

ast_node *parse_identifier_expr() {
  char *id_name = current_token.identifier_str;
  get_next_token();
  if (current_token.this_char != '(') {
    return variable_expr_ast_create(id_name);
  }

  get_next_token();
  char **args = malloc(sizeof(char *));
  unsigned int arg_count;
  if (current_token.this_char != ')') {
    while (1) {
      if (ast_node *arg = parse_expression()) {
        args[arg_count] = arg;
      } else {
        return NULL;
      }

      if (current_token.this_char == ')') {
        break;
      }

      if (current_token.this_char != ',') {
        log_error("expected ')' or ',' in argument list");
        return NULL;
      }
      get_next_token();
      arg_count++;
    }
  }

  get_next_token();

  return call_expr_ast_create(id_name, args, arg_count);
}

ast_node *parse_primary() {
  switch (current_token.this_char) {

  default:
    log_error("unknown token when expecting an expression");
    return NULL;
  case token_identifier:
    return parse_identifier_expr();

  case token_number:
    return parse_number_expr();
  case '(':
    return parse_paren_expr();
  }
}
