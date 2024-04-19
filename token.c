#include "token.h"

TokenData get_token() {
  TokenData data = {};

  static char LastChar = ' ';
  while (isspace(LastChar))
    LastChar = getchar();

  if (isalpha(LastChar)) {
    data.identifier_str[0] = LastChar;
    while (isalnum((LastChar = getchar()))) {
      data.identifier_str += LastChar;
    }

    if (strcmp(data.identifier_str, "fn") == 0) {
      data.this_char = token_def;
      return data;
    }

    if (strcmp(data.identifier_str, "extern") == 0) {
      data.this_char = token_extern;
      return data;
    }

    data.this_char = token_identifier;

    return data;
  }

  if (isdigit(LastChar) || LastChar == '.') {
    char *NumStr;
    do {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || strcmp(&LastChar, "."));
    data.num_val = strtod(NumStr, 0);
    data.this_char = token_number;
    return data;
  }

  if (strcmp(&LastChar, "//") == 0) {
    do {
      LastChar = getchar();
    } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
    if (LastChar != EOF) {
      return get_token();
    }
  }

  if (LastChar == EOF) {
    data.this_char = token_eof;
    return data;
  }

  int ThisChar = LastChar;
  LastChar = getchar();
  data.this_char = ThisChar;
  return data;
}
