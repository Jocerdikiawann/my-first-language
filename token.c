#include "token.h"

TokenData get_token() {
  TokenData data = {
      .last_char = ' ',
  };

  while (isspace(data.last_char))
    data.last_char = getchar();

  if (isalpha(data.last_char)) {
    data.identifier_str = malloc(sizeof(char) * 256);
    data.identifier_str[0] = data.last_char;
    data.identifier_str[1] = '\0';

    int i = 2;

    while (isalnum((data.last_char = getchar()))) {
      data.identifier_str[i++] = data.last_char;
    }

    data.identifier_str[i] = '\0';

    if (strcmp(data.identifier_str, "fn") == 0) {
      data.last_char = token_def;
      return data;
    }

    if (strcmp(data.identifier_str, "extern") == 0) {
      data.last_char = token_extern;
      return data;
    }

    data.last_char = token_identifier;

    return data;
  }

  if (isdigit(data.last_char) || data.last_char == '.') {
    char *NumStr;
    do {
      NumStr += data.last_char;
      data.last_char = getchar();
    } while (isdigit(data.last_char) || data.last_char == '.');
    data.num_val = strtod(NumStr, 0);
    data.last_char = token_number;
    return data;
  }

  if (data.last_char == '#') {
    do {
      data.last_char = getchar();
    } while (data.last_char != EOF && data.last_char != '\n' &&
             data.last_char != '\r');
    if (data.last_char != EOF) {
      return get_token();
    }
  }

  if (data.last_char == EOF) {
    data.last_char = token_eof;
    return data;
  }

  data.last_char = getchar();
  return data;
}
