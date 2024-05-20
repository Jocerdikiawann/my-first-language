#include "parser.h"
#include "ast.h"
#include "util.h"

static int current_token;
static TokenData token_data = {};

static BinopPrecedence binop_precedence[] = {
    {'<', 10},
    {'+', 20},
    {'-', 20},
    {'*', 40},
};

int get_next_token() { return current_token = get_token(&token_data); }

ExprAST *log_error(const char *value) {
  fprintf(stderr, "Error: %s\n", value);
  return NULL;
}

PrototypeAST *log_error_p(const char *value) {
  log_error(value);
  return NULL;
}

int get_token_precedence() {
  if (!isascii(current_token))
    return -1;

  for (size_t i = 0; *(&binop_precedence + 1) - binop_precedence; ++i) {
    if (binop_precedence[i].key == current_token &&
        binop_precedence[i].value > 0) {
      return binop_precedence[i].value;
    }
    if (binop_precedence[i].value <= 0) {
      return -1;
    }
  }
  return -1;
}

ExprAST *parse_number_expr() {
  ExprAST *result = number_expr_ast_create(token_data.num_val);
  get_next_token();
  return result;
}

ExprAST *parse_identifier_expr() {
  char id_name[token_data.identifier_str.count + 1];
  strcpy(id_name, token_data.identifier_str.items);

  get_next_token();

  if (current_token != '(') {
    return variable_expr_ast_create(id_name);
  }

  get_next_token();

  ExprASTArguments args = {};

  if (current_token != ')') {
    while (true) {
      ExprAST *arg = parse_expression();

      if (arg == NULL)
        return NULL;

      append_argument(args, arg);

      if (current_token == ')')
        break;

      if (current_token != ',')
        return log_error("expected ')' or ',' in argument list");

      get_next_token();
    }
  }

  get_next_token();

  return call_expr_ast_create(id_name, args);
}

PrototypeAST *parse_prototype() {
  if (current_token != token_identifier) {
    return log_error_p("expected function name in prototype");
  }

  char *fn_name = token_data.identifier_str.items;
  get_next_token();
  if (current_token != '(') {
    return log_error_p("expected '(' in prototype");
  }

  Arguments args = {};

  while (get_next_token() == token_identifier) {
    append(args, token_data.identifier_str.items);
  }

  if (current_token != ')') {
    return log_error_p("expected ')' in prototype");
  }

  get_next_token();

  return prototype_ast_create(fn_name, args);
}

FunctionAST *parse_definition() {
  get_next_token();
  PrototypeAST *proto = parse_prototype();
  if (!proto)
    return NULL;

  ExprAST *expr = parse_expression();
  if (expr == NULL)
    return NULL;

  return function_ast_create(proto, expr);
}

PrototypeAST *parse_extern() {
  get_next_token();
  return parse_prototype();
}

FunctionAST *parse_top_level_expr() {
  ExprAST *expr = parse_expression();
  if (!expr)
    return NULL;
  Arguments args = {};
  PrototypeAST *proto = prototype_ast_create("__anon_expr", args);
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

ExprAST *parse_binary_operations_rhs(int expr_prec, ExprAST *lhs) {
  while (true) {
    int token_prec = get_token_precedence();
    if (token_prec < expr_prec)
      return lhs;
    int binary_operations = current_token;
    get_next_token();
    ExprAST *rhs = parse_primary();
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

ExprAST *parse_primary() {
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

ExprAST *parse_expression() {
  ExprAST *lhs = parse_primary();
  if (!lhs) {
    return NULL;
  }

  return parse_binary_operations_rhs(0, lhs);
}

ExprAST *parse_paren_expr() {
  get_next_token();
  ExprAST *V = parse_expression();
  if (!V) {
    return NULL;
  }
  if (current_token != ')') {
    return log_error("expected ')'");
  }
  get_next_token();
  return V;
}

void main_loop() {

  while (true) {
    fprintf(stderr, "repl> ");
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
