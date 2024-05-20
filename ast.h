#ifndef AST_H
#define AST_H
#include "util.h"
#include <stdlib.h>
#include <string.h>

typedef struct ExprAST ExprAST;
// Arguments for dynamic allocation (append)

typedef struct {
  char **items;
  size_t count, capacity;
} Arguments;

typedef struct ExprASTArguments {
  ExprAST **items;
  size_t count, capacity;
} ExprASTArguments;

// End of arguments section

typedef struct NumberExprAST {
  double value;
} NumberExprAST;

typedef struct VariableExprAST {
  char *name;
} VariableExprAST;

typedef struct BinaryExprAST {
  char op;
  ExprAST *lhs, *rhs;
} BinaryExprAST;

typedef struct CallExprAST {
  char *callee;
  ExprASTArguments args;
} CallExprAST;

typedef struct PrototypeAST {
  char *name;
  Arguments args;
} PrototypeAST;

typedef struct FunctionAST {
  PrototypeAST *prototype;
  ExprAST *body;
} FunctionAST;

typedef struct ExprAST {
  void (*destroy)(struct ExprAST *);
  union {
    NumberExprAST number;
    VariableExprAST variable;
    BinaryExprAST binary_expr;
    CallExprAST call;
    PrototypeAST prototype;
    FunctionAST function;
  };
} ExprAST;

ExprAST *number_expr_ast_create(double value);
ExprAST *variable_expr_ast_create(char *name);
ExprAST *binary_expr_ast_create(char operations, ExprAST *lhs, ExprAST *rhs);
ExprAST *call_expr_ast_create(char *call, ExprASTArguments args);

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
PrototypeAST *prototype_ast_create(char *name, Arguments);
FunctionAST *function_ast_create(PrototypeAST *prototype, ExprAST *body);

void destroy_NumberExprAST(ExprAST *ast);
void destroy_VariableExprAST(ExprAST *ast);
void destroy_BinaryExprAST(ExprAST *ast);
void destroy_CallExprAST(ExprAST *ast);
void destroy_PrototypeAST(PrototypeAST *ast);
void destroy_FunctionAST(FunctionAST *ast);

#endif // !AST_H
