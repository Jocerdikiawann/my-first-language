#include "ast.h"
#include <string.h>

ast_node *number_expr_ast_create(double value) {
  ast_node *node = malloc(sizeof(ast_node));
  node->type = NUMBER;
  node->number.value = value;
  return node;
}

ast_node *variable_expr_ast_create(char *name) {
  ast_node *node = malloc(sizeof(ast_node));
  node->type = VARIABLE;
  node->variable.name = strdup(name);
  return node;
}

ast_node *binary_expr_ast_create(binary_operations_expression operations,
                                 ast_node *lhs, ast_node *rhs) {
  ast_node *node = malloc(sizeof(ast_node));
  node->type = BINARY_EXPR;
  node->binary_expr.operations = operations;
  node->binary_expr.lhs = lhs;
  node->binary_expr.rhs = rhs;
  return node;
}

ast_node *call_expr_ast_create(char *call, char **args, int arg_count) {
  ast_node *node = malloc(sizeof(ast_node));
  node->type = CALL;
  node->call.call = strdup(call);
  node->call.args = malloc(sizeof(ast_node) * arg_count);
  memcpy(node->prototype.args, args, arg_count);
  node->call.arg_count = arg_count;
  return node;
}
