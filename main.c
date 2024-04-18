
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

static char *IdentifierStr;
static double NumVal;

static int getToken() {
  static char LastChar = ' ';
  while (isspace(LastChar))
    LastChar = getchar();

  if (isalpha(LastChar)) {
    IdentifierStr[0] = LastChar;
    while (isalnum((LastChar = getchar()))) {
      IdentifierStr += LastChar;
    }

    if (strcmp(IdentifierStr, "fn") == 0) {
      return token_def;
    }

    if (strcmp(IdentifierStr, "extern") == 0) {
      return token_extern;
    }

    return token_identifier;
  }

  if (isdigit(LastChar) || LastChar == '.') {
    char *NumStr;
    do {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || strcmp(&LastChar, "."));
    NumVal = strtod(NumStr, 0);
    return token_number;
  }

  if (strcmp(&LastChar, "//") == 0) {
    do {
      LastChar = getchar();
    } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
    if (LastChar != EOF) {
      return getToken();
    }
  }

  if (LastChar == EOF) {
    return token_eof;
  }

  int ThisChar = LastChar;
  LastChar = getchar();
  return ThisChar;
}
