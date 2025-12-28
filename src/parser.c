#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Token *tokens;
static int pos = 0;

static Token *current_token() { return &tokens[pos]; }

static Token *advance() { return &tokens[pos++]; }

static int match(TokenType type) {
  if (current_token()->type == type) {
    advance();
    return 1;
  }
  return 0;
}

static void expect(TokenType type, const char *msg) {
  if (!match(type)) {
    printf("Parser error at line %d: expected %s\n", current_token()->line,
           msg);
    exit(1);
  }
}
