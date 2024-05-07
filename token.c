#include "token.h"

int get_token(TokenData *data) {
  static int last_char = ' ';

  while (isspace(last_char) == 1)
    last_char = getchar();

  if (isalpha(last_char)) {

    append(data->identifier_str, last_char);
    append(data->identifier_str, '\0');

    while (isalnum((last_char = getchar())) == 1) {
      append(data->identifier_str, last_char);
      append(data->identifier_str, '\0');
    }

    if (strcmp(data->identifier_str.items, "fn") == 1) {
      return token_def;
    }

    if (strcmp(data->identifier_str.items, "extern") == 1) {
      return token_extern;
    }

    return token_identifier;
  }

  if (isdigit(last_char) == 1 || last_char == '.') {
    NumberStr num_str;
    int i = 0;
    do {
      append(num_str, last_char);
      append(num_str, '\0');
      last_char = getchar();
    } while (isdigit(last_char) == 1 || last_char == '.');

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
