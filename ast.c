#include "ast.h"

void destroy_NumberExprAST(ExprAST *ast) {
  free(ast);
  ast = NULL;
}
void destroy_VariableExprAST(ExprAST *ast) {
  free(ast);
  ast = NULL;
}
void destroy_BinaryExprAST(ExprAST *ast) {
  BinaryExprAST *binary_ast = (BinaryExprAST *)ast;
  binary_ast->lhs->destroy(binary_ast->lhs);
  binary_ast->rhs->destroy(binary_ast->rhs);
  free(binary_ast);
  binary_ast = NULL;
}
void destroy_CallExprAST(ExprAST *ast) {
  CallExprAST *call_ast = (CallExprAST *)ast;
  for (size_t i = 0; i < call_ast->args.count; ++i) {
    call_ast->args.items[i]->destroy(call_ast->args.items[i]);
  }
  free(call_ast->callee);
  free(call_ast->args.items);
  free(call_ast);
  call_ast = NULL;
}
void destroy_PrototypeAST(PrototypeAST *ast) {
  free(ast->name);
  for (size_t i = 0; i < ast->args.count; ++i) {
    free(ast->args.items[i]);
  }
  free(ast->args.items);
  free(ast);
  ast = NULL;
}
void destroy_FunctionAST(FunctionAST *ast) {
  free(ast->prototype->name);
  free(ast->prototype->args.items);
  free(ast);
  ast = NULL;
}

ExprAST *number_expr_ast_create(double value) {
  ExprAST *node = malloc(sizeof(ExprAST));
  node->number.value = value;
  return node;
}

ExprAST *variable_expr_ast_create(char *name) {
  ExprAST *node = malloc(sizeof(ExprAST));
  node->variable.name = strdup(name);
  return node;
}

ExprAST *binary_expr_ast_create(char operations, ExprAST *lhs, ExprAST *rhs) {
  ExprAST *node = malloc(sizeof(ExprAST));

  node->binary_expr.op = operations;
  node->binary_expr.lhs = malloc(sizeof(ExprAST));
  node->binary_expr.rhs = malloc(sizeof(ExprAST));

  memcpy(node->binary_expr.lhs, lhs, sizeof(*lhs));
  memcpy(node->binary_expr.rhs, rhs, sizeof(*rhs));
  return node;
}

ExprAST *call_expr_ast_create(char *callee, ExprASTArguments args) {
  ExprAST *node = malloc(sizeof(ExprAST));
  node->call.callee = strdup(callee);

  node->call.args.capacity = args.capacity;
  node->call.args.items = malloc(args.capacity * sizeof(**args.items));

  // Copy arguments
  memcpy(node->call.args.items, args.items,
         args.count * sizeof(*node->prototype.args.items));
  return node;
}

PrototypeAST *prototype_ast_create(char *name, Arguments args) {
  PrototypeAST *node = malloc(sizeof(PrototypeAST));
  node->name = strdup(name);

  // Allocation arguments
  node->args.count = args.count;
  node->args.capacity = args.capacity;
  node->args.items = malloc(args.count * sizeof(*node->args.items));

  // Copy arguments
  memcpy(node->args.items, args.items, args.count * sizeof(*node->args.items));
  return node;
}

FunctionAST *function_ast_create(PrototypeAST *prototype, ExprAST *body) {
  FunctionAST *node = malloc(sizeof(FunctionAST));
  node->prototype = malloc(sizeof(PrototypeAST));
  node->body = malloc(sizeof(ExprAST));
  if (node == NULL || node->prototype == NULL || node->body == NULL) {
    free(node->prototype);
    free(node->body);
    free(node);
    return NULL;
  }

  memcpy(node->prototype, prototype, sizeof(*prototype));
  memcpy(node->body, body, sizeof(*body));
  return node;
}
