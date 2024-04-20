#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"

typedef struct {
  char key;
  int value;
} BinopPrecedence;

int get_next_token();
ast_node *parse_number_expr();
ast_node *parse_identifier_expr();
ast_node *parse_paren_expr();
ast_node *parse_primary();
ast_node *parse_expression();
ast_node *parse_binary_operations_rhs(int expr_prec, ast_node *lhs);
ast_node *parse_call_expr();
ast_node *parse_prototype();
ast_node *parse_definition();
ast_node *parse_extern();
ast_node *parse_top_level_expr();
void main_loop();

#endif // !PARSER_H
