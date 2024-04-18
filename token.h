#ifndef TOKEN_H
#define TOKEN_H

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

static int getToken();
#endif // !TOKEN_H
