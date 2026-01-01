#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Token *tokens;
static int pos = 0;

/* -------------------------------------------------- */
/* Utilities                                          */
/* -------------------------------------------------- */

static Token *current_token() { return &tokens[pos]; }
static Token *peek(int offset) { return &tokens[pos + offset]; }

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

/* -------------------------------------------------- */
/* AST helpers                                        */
/* -------------------------------------------------- */

Node *make_node(NodeType type, char *value) {
  Node *node = malloc(sizeof(Node));
  node->type = type;
  node->value = value ? strdup(value) : NULL;
  node->children = NULL;
  node->child_count = 0;
  node->return_type = TYPE_VOID;
  return node;
}

void add_child(Node *parent, Node *child) {
  parent->children =
      realloc(parent->children, sizeof(Node *) * (parent->child_count + 1));
  parent->children[parent->child_count++] = child;
}

/* -------------------------------------------------- */
/* Types                                              */
/* -------------------------------------------------- */

static DataType parse_type() {
  if (match(TOK_INT))
    return TYPE_INT;
  if (match(TOK_CHAR))
    return TYPE_CHAR;
  if (match(TOK_FLOAT))
    return TYPE_FLOAT;
  if (match(TOK_STRING))
    return TYPE_STRING;
  if (match(TOK_VOID))
    return TYPE_VOID;

  printf("Expected type at line %d\n", current_token()->line);
  exit(1);
}

/* -------------------------------------------------- */
/* Expressions                                        */
/* -------------------------------------------------- */

Node *parse_expression(); // forward

Node *parse_factor() {
  Token *t = current_token();

  if (match(TOK_INT_LIT) || match(TOK_FLOAT_LIT) || match(TOK_CHAR_LIT) ||
      match(TOK_STRING_LIT)) {
    return make_node(NODE_LITERAL, t->lexeme);
  }

  if (match(TOK_IDENT)) {
    return make_node(NODE_IDENT, t->lexeme);
  }

  if (match(TOK_LPAREN)) {
    Node *expr = parse_expression();
    expect(TOK_RPAREN, ")");
    return expr;
  }

  printf("Unexpected token '%s' at line %d\n", t->lexeme, t->line);
  exit(1);
}

Node *parse_term() {
  Node *left = parse_factor();

  while (current_token()->type == TOK_STAR ||
         current_token()->type == TOK_SLASH) {

    Token *op = advance();
    Node *bin = make_node(NODE_BINARY_OP, op->lexeme);

    add_child(bin, left);
    add_child(bin, parse_factor());

    left = bin;
  }

  return left;
}

/* comparison: > < >= <= == != */
Node *parse_comparison() {
  Node *left = parse_term();

  while (current_token()->type == TOK_GT || current_token()->type == TOK_LT ||
         current_token()->type == TOK_GTE || current_token()->type == TOK_LTE ||
         current_token()->type == TOK_EQEQ ||
         current_token()->type == TOK_NEQ) {

    Token *op = advance();
    Node *bin = make_node(NODE_BINARY_OP, op->lexeme);

    add_child(bin, left);
    add_child(bin, parse_term());

    left = bin;
  }

  return left;
}

Node *parse_expression() {
  Node *left = parse_comparison();

  while (current_token()->type == TOK_PLUS ||
         current_token()->type == TOK_MINUS) {

    Token *op = advance();
    Node *bin = make_node(NODE_BINARY_OP, op->lexeme);

    add_child(bin, left);
    add_child(bin, parse_comparison());

    left = bin;
  }

  return left;
}

/* -------------------------------------------------- */
/* Statements                                         */
/* -------------------------------------------------- */

Node *parse_statement(); // forward

Node *parse_block() {
  expect(TOK_LBRACE, "{");

  Node *block = make_node(NODE_BLOCK, NULL);

  while (!match(TOK_RBRACE)) {
    add_child(block, parse_statement());
  }

  return block;
}

Node *parse_if_statement() {
  expect(TOK_IF, "if");
  expect(TOK_LPAREN, "(");

  Node *cond = parse_expression();

  expect(TOK_RPAREN, ")");

  Node *if_node = make_node(NODE_IF, NULL);
  add_child(if_node, cond);
  add_child(if_node, parse_block());

  if (match(TOK_ELSE)) {
    add_child(if_node, parse_block());
  }

  return if_node;
}

/* function: type ident ( params ) block */
Node *parse_function_decl() {
  DataType ret_type = parse_type();

  Token *name = current_token();
  expect(TOK_IDENT, "function name");

  Node *func = make_node(NODE_FUNC_DECL, name->lexeme);
  func->return_type = ret_type;

  expect(TOK_LPAREN, "(");

  Node *params = make_node(NODE_PARAM_LIST, NULL);

  if (!match(TOK_RPAREN)) {
    do {
      DataType ptype = parse_type();
      Token *pid = current_token();
      expect(TOK_IDENT, "parameter name");

      Node *param = make_node(NODE_PARAM, pid->lexeme);
      param->return_type = ptype;
      add_child(params, param);

    } while (match(TOK_COMMA));

    expect(TOK_RPAREN, ")");
  }

  add_child(func, params);
  add_child(func, parse_block());

  return func;
}

Node *parse_statement() {

  /* function declaration */
  if ((current_token()->type == TOK_INT || current_token()->type == TOK_CHAR ||
       current_token()->type == TOK_FLOAT ||
       current_token()->type == TOK_STRING ||
       current_token()->type == TOK_VOID) &&
      peek(1)->type == TOK_IDENT && peek(2)->type == TOK_LPAREN) {

    return parse_function_decl();
  }

  /* variable declaration */
  if (current_token()->type == TOK_INT || current_token()->type == TOK_CHAR ||
      current_token()->type == TOK_FLOAT ||
      current_token()->type == TOK_STRING) {

    DataType type = parse_type();
    Token *id = current_token();
    expect(TOK_IDENT, "identifier");

    Node *decl = make_node(NODE_VAR_DECL, id->lexeme);
    decl->return_type = type;
    if (match(TOK_EQ)) {
      add_child(decl, parse_expression());
    }

    expect(TOK_SEMI, ";");
    return decl;
  }

  /* if statement */
  if (current_token()->type == TOK_IF) {
    return parse_if_statement();
  }

  /* return */
  if (match(TOK_RETURN)) {
    Node *ret = make_node(NODE_RETURN, NULL);
    add_child(ret, parse_expression());
    expect(TOK_SEMI, ";");
    return ret;
  }

  /* assignment */
  if (current_token()->type == TOK_IDENT && peek(1)->type == TOK_EQ) {

    Token *id = advance();
    advance(); // '='

    Node *assign = make_node(NODE_ASSIGN, id->lexeme);
    add_child(assign, parse_expression());

    expect(TOK_SEMI, ";");
    return assign;
  }

  /* block */
  if (current_token()->type == TOK_LBRACE) {
    return parse_block();
  }

  printf("Invalid statement at line %d (token: %s)\n", current_token()->line,
         current_token()->lexeme);
  exit(1);
}

/* -------------------------------------------------- */
/* Program                                            */
/* -------------------------------------------------- */

Node *parse_program() {
  Node *program = make_node(NODE_PROGRAM, NULL);

  while (current_token()->type != TOK_EOF) {
    add_child(program, parse_statement());
  }

  return program;
}

Node *parser(Token *toks) {
  tokens = toks;
  pos = 0;
  return parse_program();
}
