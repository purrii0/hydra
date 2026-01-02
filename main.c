#include "../include/parser.h"
#include "../include/tokenizer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: hydra <file.hy>\n");
    return 1;
  }

  FILE *f = fopen(argv[1], "r");
  if (!f) {
    perror("fopen");
    return 1;
  }

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  char *source = malloc(size + 1);
  fread(source, 1, size, f);
  source[size] = 0;
  fclose(f);

  Token *tokens = tokenizer(source);
  Node *ast = parser(tokens);

  generate_c(ast, "out.c");

  printf("Generated out.c\n");
  printf("Compile with: gcc out.c -o out\n");

  return 0;
}
