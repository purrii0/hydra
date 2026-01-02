#ifndef PARSER_H
#define PARSER_H

#include "./tokenizer.h"

typedef enum {
  NODE_PROGRAM,
  NODE_FUNC_DECL,
  NODE_BLOCK,
  NODE_PARAM_LIST,
  NODE_PARAM,
  NODE_VAR_DECL,
  NODE_ASSIGN,
  NODE_IF,
  NODE_WHILE,
  NODE_FOR,
  NODE_RETURN,
  NODE_BINARY_OP,
  NODE_UNARY_OP,
  NODE_LITERAL,
  NODE_PRINT,
  NODE_IDENT
} NodeType;

typedef enum {
  TYPE_INT,
  TYPE_CHAR,
  TYPE_STRING,
  TYPE_FLOAT,
  TYPE_VOID
} DataType;

typedef struct {
  DataType type;
  char *identifier;
  char *value;
} Params;

typedef struct Node {
  NodeType type;
  struct Node **children;
  int child_count;
  char *value;

  DataType return_type;
  Params params[64];
} Node;

Node *parse_expression();
Node *parse_term();
Node *parse_factor();
Node *parse_statement();
Node *parse_program();

Node *make_node(NodeType type, char *value);
void add_child(Node *parent, Node *child);

Node *parser(Token *toks);

void generate_c(Node *ast, const char *output_file);

#endif
