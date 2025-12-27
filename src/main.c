#include "../include/file_reader.h"
#include "../include/tokenizer.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char source[65536];

  if (argc < 2) {
    printf("Provide a File\n");
    return 1;
  }

  read_file(argv[1], source);

  Token *tokens = tokenizer(source);

  for (int i = 0; tokens[i].type != TOK_EOF; i++) {
    printf("%s ", token_type_to_string(tokens[i].type));
    printf("%s ", tokens[i].lexeme);
    printf("%d \n", tokens[i].line);
  }

  return 0;
}
