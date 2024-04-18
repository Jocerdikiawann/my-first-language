#include "ast.h"
#include <stdlib.h>

ast_node *number_expr_ast_create(double value) {
  ast_node *node = malloc(sizeof(ast_node));
  node->type = NUMBER;
  node->number.value = value;
  return node;
}
