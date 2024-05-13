#include "token.h"

int get_token(TokenData *data) {
  static int last_char = ' ';

  while (isspace(last_char))
    last_char = getchar();

  if (isalpha(last_char)) {

    append(data->identifier_str, last_char);

    while (isalnum((last_char = getchar()))) {
      append(data->identifier_str, last_char);
    }

    if (strcmp(data->identifier_str.items, "fn")) {
      return token_def;
    }

    if (strcmp(data->identifier_str.items, "extern")) {
      return token_extern;
    }

    return token_identifier;
  }

  if (isdigit(last_char) || last_char == '.') {
    NumberStr num_str = {};

    do {
      append(num_str, last_char);
      last_char = getchar();
    } while (isdigit(last_char) == true || last_char == '.');

    data->num_val = strtod(num_str.items, NULL);
    return token_number;
  }

  if (last_char == '#') {
    do {
      last_char = getchar();
    } while (last_char != EOF && last_char != '\n' && last_char != '\r');
    if (last_char != EOF) {
      return get_token(data);
    }
  }

  if (last_char == EOF) {
    return token_eof;
  }

  int this_char = last_char;
  last_char = getchar();
  return this_char;
}
