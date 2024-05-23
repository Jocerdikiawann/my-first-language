#include "token.h"

int get_token(TokenData *data) {
  static int last_char = ' ';

  // Skip any whitespace
  while (isspace(last_char) == 1)
    last_char = getchar();

  if (isalpha(last_char)) {
    append(data->identifier_str, last_char);
    while (isalnum((last_char = getchar())))
      append(data->identifier_str, last_char);
    append(data->identifier_str, '\0');

    // if identifier is 'fn' return token_def
    if (strcmp(data->identifier_str.items, "fn") == 0) {
      return token_def;
    }

    if (strcmp(data->identifier_str.items, "extern") == 0) {
      return token_extern;
    }

    return token_identifier;
  }

  if (isdigit(last_char) || last_char == '.') {
    NumberStr num_str;
    do {
      append(num_str, last_char);
      last_char = getchar();
    } while (isdigit(last_char) || last_char == '.');

    append(num_str, '\0');
    char *end_num_str;
    data->num_val = strtod(num_str.items, &end_num_str);
    if (end_num_str == num_str.items || *end_num_str != '\0') {
      fprintf(stderr, "Error: Invalid number\n");
      exit(1);
    }
    return token_number;
  }

  // if last_char is a comment // skip until newline
  if (last_char == '/') {
    if ((last_char = getchar()) == '/') {
      do {
        last_char = getchar();
      } while (last_char != EOF && last_char != '\n' && last_char != '\r');
      if (last_char != EOF)
        return get_token(data);
    }
  }

  if (last_char == EOF)
    return token_eof;

  int this_char = last_char;
  last_char = getchar();
  return this_char;
}
