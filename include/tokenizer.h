#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum {
  TOK_INT,
  TOK_CHAR,
  TOK_STRING,
  TOK_FLOAT,
  TOK_VOID,
  TOK_IF,
  TOK_ELSE,
  TOK_WHILE,
  TOK_FOR,
  TOK_RETURN,
  TOK_IDENT,
  TOK_INT_LIT,
  TOK_FLOAT_LIT,
  TOK_CHAR_LIT,
  TOK_STRING_LIT,
  TOK_PLUS,
  TOK_MINUS,
  TOK_STAR,
  TOK_SLASH,
  TOK_EQ,
  TOK_EQEQ,
  TOK_LT,
  TOK_GT,
  TOK_NEQ,
  TOK_LTE,
  TOK_GTE,
  TOK_PRINT,
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_LBRACE,
  TOK_RBRACE,
  TOK_SEMI,
  TOK_COMMA,
  TOK_EOF
} TokenType;

typedef struct {
  TokenType type;
  char lexeme[256];
  int line;
} Token;

Token *tokenizer(const char *source);
TokenType get_token_type(char *token);
const char *token_type_to_string(TokenType type);

#endif
