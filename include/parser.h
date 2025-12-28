#ifndef PARSER_H
#define PARSER_H

#include "./tokenizer.h"

typedef enum {
  NODE_PROGRAM,
  NODE_FUNC_DECL,
  NODE_VAR_DECL,
  NODE_ASSIGN,
  NODE_IF,
  NODE_WHILE,
  NODE_FOR,
  NODE_RETURN,
  NODE_BINARY_OP,
  NODE_UNARY_OP,
  NODE_LITERAL,
  NODE_IDENT
} NodeType;

typedef struct Node {
  NodeType type;
  struct Node **children;
  int child_count;
  char *value;
} Node;

Node *parse_expression();
Node *parse_term();
Node *parse_factor();
Node *parse_statement();
Node *parse_program();

Node *make_child(NodeType type, char *value);
void add_child(Node *parent, Node *child);

#endif
