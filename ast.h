#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <string.h>

struct ast_node;
typedef enum ast_node_type_e {
  NUMBER,
  VARIABLE,
  BINARY_EXPR,
  CALL,
  PROTOTYPE,
  FUNCTION,
} ast_node_type_e;

typedef enum binary_operations_expression {
  ADD,
  SUB,
  MUL,
  DIV,
} binary_operations_expression;

typedef struct {
  double value;
} number_expr_ast;

typedef struct {
  char *name;
} variable_expr_ast;

typedef struct {
  binary_operations_expression operations;
  struct ast_node *lhs, *rhs;
} binary_expr_ast;

typedef struct {
  char *call;
  struct ast_node **args;
  unsigned int arg_count;
} call_expr_ast;

typedef struct {
  char *name;
  char **args;
  unsigned int arg_count;
} prototype_ast;

typedef struct {
  struct ast_node *prototype, *body;
} function_ast;

typedef struct ast_node {
  ast_node_type_e type;
  union {
    number_expr_ast number;
    variable_expr_ast variable;
    binary_expr_ast binary_expr;
    call_expr_ast call;
    prototype_ast prototype;
    function_ast function;
  };
} ast_node;

ast_node *number_expr_ast_create(double value);
ast_node *variable_expr_ast_create(char *name);
ast_node *binary_expr_ast_create(binary_operations_expression operations,
                                 ast_node *lhs, ast_node *rhs);
ast_node *call_expr_ast_create(char *call, char **args, int arg_count);
ast_node *prototype_ast_create(char *name, char **args, int arg_count);
ast_node *function_ast_create(ast_node *prototype, ast_node *body);
void node_ast_free(struct ast_node *node);

#endif // !AST_H
