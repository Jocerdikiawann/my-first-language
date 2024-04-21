#include "token.h"

TokenData get_token() {
  TokenData data = {};

  static char last_char = ' ';
  while (isspace(last_char))
    last_char = getchar();

  if (isalpha(last_char)) {
    data.identifier_str[0] = last_char;
    int i = 1;
    while (isalnum((last_char = getchar()))) {
      data.identifier_str[i++] = last_char;
    }
    data.identifier_str[i] = '\0';

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

  if (isdigit(last_char) || last_char == '.') {
    char *NumStr;
    do {
      NumStr += last_char;
      last_char = getchar();
    } while (isdigit(last_char) || strcmp(&last_char, "."));
    data.num_val = strtod(NumStr, 0);
    data.this_char = token_number;
    return data;
  }

  if (strcmp(&last_char, "//") == 0) {
    do {
      last_char = getchar();
    } while (last_char != EOF && last_char != '\n' && last_char != '\r');
    if (last_char != EOF) {
      return get_token();
    }
  }

  if (last_char == EOF) {
    data.this_char = token_eof;
    return data;
  }

  int this_char = last_char;
  last_char = getchar();
  data.this_char = this_char;
  return data;
}
