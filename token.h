#ifndef TOKEN_H
#define TOKEN_H

#include "util.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Token {
  token_eof = -1,
  token_def = -2,
  token_extern = -3,
  token_identifier = -4,
  token_number = -5,
};

typedef struct {
  char *items;
  size_t count, capacity;
} IdentifierStr;

typedef struct {
  char *items;
  size_t count, capacity;
} NumberStr;

typedef struct {
  int num_val;
  IdentifierStr identifier_str;
} TokenData;

int get_token(TokenData *data);
#endif // !TOKEN_H
