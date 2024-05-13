#include "parser.h"

static int current_token;
static BinopPrecedence binop_precedence[] = {{'<', 10}, {'>', 10}, {'+', 20},
                                             {'-', 20}, {'*', 40}, {'/', 40}};

ast_node *log_error(const char *value) {
  fprintf(stderr, "Error: %s\n", value);
  return NULL;
}

static ast_node *get_lhs() { return variable_expr_ast_create("x"); }
static ast_node *get_rhs() { return variable_expr_ast_create("y"); }
static ast_node *get_result() {
  return binary_expr_ast_create(ADD, get_lhs(), get_rhs());
}

static TokenData token_data = {};

void get_next_token() {
  current_token = get_token(&token_data);
  return;
}

int get_token_precedence() {
  if (!isascii(current_token))
    return -1;

  int token_prec = binop_precedence[current_token].value;
  if (token_prec <= 0)
    return -1;
  return token_prec;
}

ast_node *parse_number_expr() {
  ast_node *result = number_expr_ast_create(token_data.num_val);
  get_next_token();
  return result;
}

ast_node *parse_identifier_expr() {
  char *id_name = token_data.identifier_str.items;
  get_next_token();
  if (current_token != '(') {
    return variable_expr_ast_create(id_name);
  }

  get_next_token();

  Arguments args = {};

  if (current_token != ')') {
    while (true) {
      ast_node *arg = parse_expression();
      if (arg != NULL)
        append_argument(args, arg);
      else
        return NULL;

      if (current_token == ')') {
        break;
      }

      if (current_token != ',') {
        return log_error("expected ')' or ',' in argument list");
      }
      get_next_token();
    }
  }

  get_next_token();

  return call_expr_ast_create(id_name, args.items, args.count);
}

ast_node *parse_prototype() {
  if (current_token != token_identifier) {
    return log_error("expected function name in prototype");
  }

  char *fn_name = token_data.identifier_str.items;
  get_next_token();
  if (current_token != '(') {
    return log_error("expected '(' in prototype");
  }

  ArgumentNames args = {};

  while (current_token == token_identifier) {
    append(args, token_data.identifier_str.items);
  }

  if (current_token != ')') {
    return log_error("expected ')' in prototype");
  }

  get_next_token();

  return prototype_ast_create(fn_name, args.items, args.count);
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
  char *args[] = {};
  ast_node *proto = prototype_ast_create("__anon_expr", args, 0);
  return function_ast_create(proto, expr);
}

void handle_definition() {
  if (parse_definition()) {
    fprintf(stderr, "Parsed a function definition.\n");
  } else {
    get_next_token();
  }
}

void handle_extern() {
  if (parse_extern()) {
    fprintf(stderr, "Parsed in extern.\n");
  } else {
    get_next_token();
  }
}

void handle_top_level_expression() {
  if (parse_top_level_expr()) {
    fprintf(stderr, "Parsed top level expression.\n");
  } else {
    get_next_token();
  }
}

void main_loop() {
  while (true) {
    printf("curtok: %d\n", current_token);
    printf("identifier_str: %s\n", token_data.identifier_str.items);
    printf("num_val: %d\n", token_data.num_val);
    switch (current_token) {
    case token_eof:
      return;
    case ';':
      get_next_token();
      break;
    case token_def:
      handle_definition();
      break;
    case token_extern:
      handle_extern();
      break;
    default:
      handle_top_level_expression();
      break;
    }
  }
}

ast_node *parse_binary_operations_rhs(int expr_prec, ast_node *lhs) {
  while (1) {
    int token_prec = get_token_precedence();
    if (token_prec < expr_prec)
      return lhs;
    int binary_operations = current_token;
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
  switch (current_token) {

  default:
    return log_error("unknown token when expecting an expression");
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
  if (current_token != ')') {
    return log_error("expected ')'");
  }
  get_next_token();
  return V;
}
