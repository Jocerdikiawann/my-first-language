#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"

typedef struct {
  char key;
  int value;
} BinopPrecedence;

int get_next_token();
ExprAST *parse_number_expr();
ExprAST *parse_identifier_expr();
ExprAST *parse_paren_expr();
ExprAST *parse_primary();
ExprAST *parse_expression();
ExprAST *parse_binary_operations_rhs(int expr_prec, ExprAST *lhs);
ExprAST *parse_call_expr();

PrototypeAST *parse_prototype();
FunctionAST *parse_definition();
FunctionAST *parse_top_level_expr();
PrototypeAST *parse_extern();
void handle_definition();
void handle_extern();
void handle_top_level_expression();
void main_loop();

#endif // !PARSER_H
