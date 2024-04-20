#include "ast.h"

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

ast_node *call_expr_ast_create(char *call, ast_node **args, int arg_count) {
  ast_node *node = malloc(sizeof(ast_node));
  node->type = CALL;
  node->call.call = strdup(call);
  node->call.args = malloc(sizeof(ast_node) * arg_count);
  memcpy(node->prototype.args, args, arg_count * sizeof(char *));
  node->call.arg_count = arg_count;
  return node;
}

ast_node *prototype_ast_create(char *name, char **args, int arg_count) {
  int i;
  ast_node *node = malloc(sizeof(ast_node));
  node->type = PROTOTYPE;
  node->prototype.name = strdup(name);

  node->prototype.args = malloc(sizeof(char *) * arg_count);
  for (i = 0; i < arg_count; ++i) {
    node->prototype.args[i] = strdup(args[i]);
  }
  node->prototype.arg_count = arg_count;
  return node;
}

ast_node *function_ast_create(ast_node *prototype, ast_node *body) {
  ast_node *node = malloc(sizeof(ast_node));
  node->type = FUNCTION;
  node->function.prototype = prototype;
  node->function.body = body;
  return node;
}

void node_ast_free(ast_node *node) {
  unsigned int i;
  if (!node)
    return;
  switch (node->type) {
  case NUMBER:
    break;
  case VARIABLE:
    if (node->variable.name)
      free(node->variable.name);
    break;
  case BINARY_EXPR:
    if (node->binary_expr.lhs)
      node_ast_free(node->binary_expr.lhs);
    if (node->binary_expr.rhs)
      node_ast_free(node->binary_expr.rhs);
    break;
  case CALL:
    if (node->call.call)
      free(node->call.call);
    for (i = 0; i < node->call.arg_count; ++i) {
      node_ast_free(node->call.args[i]);
    }
    free(node->call.args);
    break;
  case PROTOTYPE:
    if (node->prototype.name)
      free(node->prototype.name);
    for (i = 0; i < node->prototype.arg_count; ++i) {
      free(node->prototype.args[i]);
    }
    free(node->prototype.args);
    break;
  case FUNCTION:
    if (node->function.prototype)
      node_ast_free(node->function.prototype);
    if (node->function.body)
      node_ast_free(node->function.body);
    break;
  }
}
