#ifndef AST_H
#define AST_H

struct Ast_Node;

typedef enum BinaryOperationsExpression {
  ADD,
  SUB,
  MUL,
  DIV,
} BinaryOperationsExpression;

typedef struct {
  double value;
} NumberExprAST;

typedef struct {
  char *name;
} VariableExprAST;

typedef struct {
  BinaryOperationsExpression operations;
  struct Ast_Node *lhs, *rhs;
} BinaryExprAST;

typedef struct {
  char *calle;
  struct Ast_Node **args;
  unsigned int arg_count;
} CallExprAST;

typedef struct {
  char *name;
  char **args;
  unsigned int arg_count;
} PrototypeAST;

typedef struct {
  struct Ast_Node *prototype, *body;
} FunctionAST;

#endif // !AST_H
