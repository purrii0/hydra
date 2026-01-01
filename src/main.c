#include "../include/file_reader.h"
#include "../include/parser.h"
#include "../include/tokenizer.h"
#include <stdio.h>

const char *node_type_to_string(NodeType type) {
  switch (type) {
  case NODE_PROGRAM:
    return "PROGRAM";
  case NODE_FUNC_DECL:
    return "FUNC_DECL";
  case NODE_BLOCK:
    return "BLOCK";
  case NODE_PARAM_LIST:
    return "PARAM_LIST";
  case NODE_PARAM:
    return "PARAM";
  case NODE_VAR_DECL:
    return "VAR_DECL";
  case NODE_ASSIGN:
    return "ASSIGN";
  case NODE_IF:
    return "IF";
  case NODE_WHILE:
    return "WHILE";
  case NODE_FOR:
    return "FOR";
  case NODE_RETURN:
    return "RETURN";
  case NODE_BINARY_OP:
    return "BINARY_OP";
  case NODE_UNARY_OP:
    return "UNARY_OP";
  case NODE_LITERAL:
    return "LITERAL";
  case NODE_IDENT:
    return "IDENT";
  default:
    return "UNKNOWN";
  }
}

const char *data_type_to_string(DataType type) {
  switch (type) {
  case TYPE_INT:
    return "int";
  case TYPE_CHAR:
    return "char";
  case TYPE_STRING:
    return "string";
  case TYPE_FLOAT:
    return "float";
  case TYPE_VOID:
    return "void";
  default:
    return "unknown";
  }
}

void print_ast(Node *node, int indent) {
  if (!node)
    return;

  // indentation
  for (int i = 0; i < indent; i++)
    printf("  ");

  printf("%s", node_type_to_string(node->type));

  if (node->value)
    printf(" [%s]", node->value);

  if (node->return_type != TYPE_VOID)
    printf(" : %s", data_type_to_string(node->return_type));

  printf("\n");

  for (int i = 0; i < node->child_count; i++) {
    print_ast(node->children[i], indent + 1);
  }
}

int main(int argc, char *argv[]) {
  char source[65536];

  if (argc < 2) {
    printf("Provide a File\n");
    return 1;
  }

  read_file(argv[1], source);

  Token *tokens = tokenizer(source);

  Node *ast = parser(tokens);

  print_ast(ast, 0);

  return 0;
}
