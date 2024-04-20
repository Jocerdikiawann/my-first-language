#include "parser.h"
#include "ast.h"

static TokenData current_token;
static BinopPrecedence binop_precedence[] = {{'<', 10}, {'>', 10}, {'+', 20},
                                             {'-', 20}, {'*', 40}, {'/', 40}};

void log_error(const char *value) { fprintf(stderr, "Error: %s\n", value); }
void log_errorp(const char *value) { log_error(value); }

static ast_node *get_lhs() { return variable_expr_ast_create("x"); }
static ast_node *get_rhs() { return variable_expr_ast_create("y"); }
static ast_node *get_result() {
  return binary_expr_ast_create(ADD, get_lhs(), get_rhs());
}

int get_next_token() {
  current_token = get_token();
  return current_token.this_char;
}

int get_token_precedence() {
  if (!isascii(current_token.this_char))
    return -1;

  int token_prec = binop_precedence[current_token.this_char].value;
  if (token_prec <= 0)
    return -1;
  return token_prec;
}

ast_node *parse_number_expr() {
  ast_node *result = number_expr_ast_create(current_token.num_val);
  get_next_token();
  return result;
}

ast_node *parse_identifier_expr() {
  char *id_name = current_token.identifier_str;
  get_next_token();
  if (current_token.this_char != '(') {
    return variable_expr_ast_create(id_name);
  }

  get_next_token();

  ast_node **args = malloc(sizeof(ast_node));

  if (args == NULL)
    return NULL;
  int arg_count = 0;
  if (current_token.this_char != ')') {
    while (1) {
      ast_node *arg = parse_expression();
      if (arg) {
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

  ast_node *act_args[arg_count];
  memcpy(act_args, args, arg_count * sizeof(ast_node *));

  free(args);
  args = NULL;

  return call_expr_ast_create(id_name, act_args, arg_count);
}

ast_node *parse_prototype() {
  if (current_token.this_char != token_identifier) {
    log_error("expected function name in prototype");
    return NULL;
  }

  char *fn_name = current_token.identifier_str;
  get_next_token();

  if (current_token.this_char != '(') {
    log_error("expected '(' in prototype");
    return NULL;
  }

  char **args = malloc(sizeof(char *));
  if (args == NULL)
    return NULL;

  int arg_count = 0;
  while (get_next_token() == token_identifier) {
    args[arg_count] = current_token.identifier_str;
    arg_count++;
  }

  if (current_token.this_char != ')') {
    log_error("expected ')' in prototype");
    return NULL;
  }

  get_next_token();

  return prototype_ast_create(fn_name, args, arg_count);
}

ast_node *parse_definition() {
  get_next_token();
  ast_node *proto = parse_prototype();
  if (!proto)
    return NULL;

  ast_node *expr = parse_expression();
  if (!expr)
    return NULL;

  return function_ast_create(proto, expr);
}

ast_node *parse_extern() {
  get_next_token();
  return parse_prototype();
}

ast_node *parse_top_level_expr() {
  ast_node *expr = parse_expression();
  if (!expr)
    return NULL;

  ast_node *proto = prototype_ast_create("", NULL, 0);
  return function_ast_create(proto, expr);
}

void main_loop() {
  while (1) {
    fprintf(stderr, "ready> ");
    switch (current_token.this_char) {
    case token_eof:
      return;
    case ';':
      get_next_token();
      break;
    case token_def:
      parse_definition();
      break;
    case token_extern:
      parse_extern();
      break;
    default:
      parse_top_level_expr();
      break;
    }
  }
}

ast_node *parse_binary_operations_rhs(int expr_prec, ast_node *lhs) {
  while (1) {
    int token_prec = get_token_precedence();
    if (token_prec < expr_prec)
      return lhs;
    int binary_operations = current_token.this_char;
    get_next_token();
    ast_node *rhs = parse_primary();
    if (!rhs)
      return NULL;

    int next_prec = get_token_precedence();
    if (token_prec < next_prec) {
      rhs = parse_binary_operations_rhs(token_prec + 1, rhs);
      if (!rhs)
        return NULL;
    }
    lhs = binary_expr_ast_create(binary_operations, lhs, rhs);
  }
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

ast_node *parse_expression() {
  ast_node *lhs = parse_primary();
  if (!lhs) {
    return NULL;
  }

  return parse_binary_operations_rhs(0, lhs);
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
