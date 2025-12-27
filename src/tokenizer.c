#include "../include/tokenizer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 4096
#define MAX_LEXEME_LENGTH 256

Token *tokenizer(const char *source) {
  Token *tokens = malloc(sizeof(Token) * MAX_TOKENS);
  if (!tokens) {
    perror("malloc failed");
    return NULL;
  }
  int token_count = 0;
  int line = 0;

  for (int i = 0; source[i] != '\0'; i++) {
    char c = source[i];

    if (c == ' ' || c == '\t')
      continue;
    if (c == '\n') {
      line++;
      continue;
    }

    if (token_count >= MAX_TOKENS - 1) {
      printf("Error: Too many tokens (max %d)\n", MAX_TOKENS);
      free(tokens);
      return NULL;
    }

    if (isdigit(c)) {
      int j = 0;
      int dot_seen = 0;
      while ((isdigit(source[i]) || source[i] == '.') &&
             j < MAX_LEXEME_LENGTH - 1) {
        if (source[i] == '.' && dot_seen) {
          printf("Error: Multiple decimal points in line no. %d\n", line);
          free(tokens);
          return NULL;
        }
        if (source[i] == '.')
          dot_seen = 1;
        tokens[token_count].lexeme[j++] = source[i++];
      }
      tokens[token_count].lexeme[j] = '\0';
      tokens[token_count].line = line;
      if (dot_seen) {
        tokens[token_count].type = TOK_FLOAT_LIT;
      } else {
        tokens[token_count].type = TOK_INT_LIT;
      }
      token_count++;
      i--;
      continue;
    } else if (isalpha(c) || c == '_') {
      int j = 0;
      while ((isalnum(source[i]) || source[i] == '_') &&
             j < MAX_LEXEME_LENGTH - 1) {
        tokens[token_count].lexeme[j++] = source[i++];
      }
      tokens[token_count].lexeme[j] = '\0';
      tokens[token_count].line = line;
      tokens[token_count].type = get_token_type(tokens[token_count].lexeme);
      token_count++;
      i--;
      continue;
    } else if (c == ';' || c == ',') {
      tokens[token_count].lexeme[0] = c;
      tokens[token_count].lexeme[1] = '\0';
      tokens[token_count].line = line;
      tokens[token_count].type = get_token_type(tokens[token_count].lexeme);
      token_count++;
      continue;
    } else if (c == ')' || c == '(' || c == '{' || c == '}') {
      tokens[token_count].lexeme[0] = c;
      tokens[token_count].lexeme[1] = '\0';
      tokens[token_count].line = line;
      tokens[token_count].type = get_token_type(tokens[token_count].lexeme);
      token_count++;
      continue;
    } else if (c == '<') {
      if (source[i + 1] == '=') {
        tokens[token_count].lexeme[0] = '<';
        tokens[token_count].lexeme[1] = '=';
        tokens[token_count].lexeme[2] = '\0';
        tokens[token_count].type = TOK_LTE;
        tokens[token_count].line = line;
        token_count++;
        i++;
        continue;
      } else {
        tokens[token_count].lexeme[0] = '<';
        tokens[token_count].lexeme[1] = '\0';
        tokens[token_count].type = TOK_LT;
        tokens[token_count].line = line;
        token_count++;
        continue;
      }
    } else if (c == '>') {
      if (source[i + 1] == '=') {
        tokens[token_count].lexeme[0] = '>';
        tokens[token_count].lexeme[1] = '=';
        tokens[token_count].lexeme[2] = '\0';
        tokens[token_count].type = TOK_GTE;
        tokens[token_count].line = line;
        token_count++;
        i++;
        continue;
      } else {
        tokens[token_count].lexeme[0] = '>';
        tokens[token_count].lexeme[1] = '\0';
        tokens[token_count].type = TOK_GT;
        tokens[token_count].line = line;
        token_count++;
        continue;
      }
    } else if (c == '=') {
      if (source[i + 1] == '=') {
        tokens[token_count].lexeme[0] = '=';
        tokens[token_count].lexeme[1] = '=';
        tokens[token_count].lexeme[2] = '\0';
        tokens[token_count].type = TOK_EQEQ;
        tokens[token_count].line = line;
        token_count++;
        i++;
        continue;
      } else {
        tokens[token_count].lexeme[0] = '=';
        tokens[token_count].lexeme[1] = '\0';
        tokens[token_count].type = TOK_EQ;
        tokens[token_count].line = line;
        token_count++;
        continue;
      }
    } else if (c == '!' && source[i + 1] == '=') {
      tokens[token_count].lexeme[0] = '!';
      tokens[token_count].lexeme[1] = '=';
      tokens[token_count].lexeme[2] = '\0';
      tokens[token_count].type = TOK_NEQ;
      tokens[token_count].line = line;
      token_count++;
      i++;
      continue;
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
      tokens[token_count].lexeme[0] = c;
      tokens[token_count].lexeme[1] = '\0';
      tokens[token_count].type = get_token_type(tokens[token_count].lexeme);
      tokens[token_count].line = line;
      token_count++;
      continue;
    } else if (c == '\'') {
      if (source[i + 1] != '\0' && source[i + 1] != '\n' &&
          source[i + 2] == '\'') {
        tokens[token_count].lexeme[0] = c;
        tokens[token_count].lexeme[1] = source[i + 1];
        tokens[token_count].lexeme[2] = source[i + 2];
        tokens[token_count].lexeme[3] = '\0';
        tokens[token_count].type = TOK_CHAR_LIT;
        tokens[token_count].line = line;
        token_count++;
        i += 2;
        continue;
      } else {
        printf("Error: Invalid character literal at line %d\n", line);
        free(tokens);
        return NULL;
      }
    } else if (c == '"') {
      int j = 1;
      tokens[token_count].lexeme[0] = c;
      i++;
      while (source[i] != '"' && j < MAX_LEXEME_LENGTH - 2) {
        if (source[i] == '\0' || source[i] == '\n') {
          printf("Error: Unterminated string literal at line %d\n", line);
          free(tokens);
          return NULL;
        }
        tokens[token_count].lexeme[j++] = source[i++];
      }
      if (source[i] != '"') {
        printf("Error: String literal too long at line %d\n", line);
        free(tokens);
        return NULL;
      }
      tokens[token_count].lexeme[j] = '"';
      tokens[token_count].lexeme[j + 1] = '\0';
      tokens[token_count].type = TOK_STRING_LIT;
      tokens[token_count].line = line;
      token_count++;
      continue;
    } else {
      printf("Unexpected character '%c' at line %d\n", c, line);
      free(tokens);
      return NULL;
    }
  }

  strcpy(tokens[token_count].lexeme, "EOF");
  tokens[token_count].type = TOK_EOF;
  tokens[token_count].line = line;

  return tokens;
}

TokenType get_token_type(char *token) {
  if (strcmp(token, "int") == 0)
    return TOK_INT;
  if (strcmp(token, "char") == 0)
    return TOK_CHAR;
  if (strcmp(token, "string") == 0)
    return TOK_STRING;
  if (strcmp(token, "float") == 0)
    return TOK_FLOAT;
  if (strcmp(token, "void") == 0)
    return TOK_VOID;
  if (strcmp(token, "if") == 0)
    return TOK_IF;
  if (strcmp(token, "else") == 0)
    return TOK_ELSE;
  if (strcmp(token, "while") == 0)
    return TOK_WHILE;
  if (strcmp(token, "for") == 0)
    return TOK_FOR;
  if (strcmp(token, "return") == 0)
    return TOK_RETURN;
  if (strcmp(token, "main") == 0)
    return TOK_MAIN;

  if (strcmp(token, "+") == 0)
    return TOK_PLUS;
  if (strcmp(token, "-") == 0)
    return TOK_MINUS;
  if (strcmp(token, "*") == 0)
    return TOK_STAR;
  if (strcmp(token, "/") == 0)
    return TOK_SLASH;
  if (strcmp(token, "=") == 0)
    return TOK_EQ;
  if (strcmp(token, "==") == 0)
    return TOK_EQEQ;
  if (strcmp(token, "<") == 0)
    return TOK_LT;
  if (strcmp(token, ">") == 0)
    return TOK_GT;
  if (strcmp(token, "!=") == 0)
    return TOK_NEQ;
  if (strcmp(token, "<=") == 0)
    return TOK_LTE;
  if (strcmp(token, ">=") == 0)
    return TOK_GTE;
  if (strcmp(token, "(") == 0)
    return TOK_LPAREN;
  if (strcmp(token, ")") == 0)
    return TOK_RPAREN;
  if (strcmp(token, "{") == 0)
    return TOK_LBRACE;
  if (strcmp(token, "}") == 0)
    return TOK_RBRACE;
  if (strcmp(token, ";") == 0)
    return TOK_SEMI;
  if (strcmp(token, ",") == 0)
    return TOK_COMMA;

  if (token[0] >= '0' && token[0] <= '9')
    return TOK_INT_LIT;
  if (token[0] == '\'' && token[2] == '\'')
    return TOK_CHAR_LIT;
  if (token[0] == '"' && token[strlen(token) - 1] == '"')
    return TOK_STRING_LIT;

  return TOK_IDENT;
}

const char *token_type_to_string(TokenType type) {
  switch (type) {
  case TOK_INT:
    return "TOK_INT";
  case TOK_CHAR:
    return "TOK_CHAR";
  case TOK_STRING:
    return "TOK_STRING";
  case TOK_FLOAT:
    return "TOK_FLOAT";
  case TOK_VOID:
    return "TOK_VOID";
  case TOK_IF:
    return "TOK_IF";
  case TOK_ELSE:
    return "TOK_ELSE";
  case TOK_WHILE:
    return "TOK_WHILE";
  case TOK_FOR:
    return "TOK_FOR";
  case TOK_RETURN:
    return "TOK_RETURN";
  case TOK_MAIN:
    return "TOK_MAIN";
  case TOK_IDENT:
    return "TOK_IDENT";
  case TOK_INT_LIT:
    return "TOK_INT_LIT";
  case TOK_FLOAT_LIT:
    return "TOK_FLOAT_LIT";
  case TOK_CHAR_LIT:
    return "TOK_CHAR_LIT";
  case TOK_STRING_LIT:
    return "TOK_STRING_LIT";
  case TOK_PLUS:
    return "TOK_PLUS";
  case TOK_MINUS:
    return "TOK_MINUS";
  case TOK_STAR:
    return "TOK_STAR";
  case TOK_SLASH:
    return "TOK_SLASH";
  case TOK_EQ:
    return "TOK_EQ";
  case TOK_EQEQ:
    return "TOK_EQEQ";
  case TOK_LT:
    return "TOK_LT";
  case TOK_GT:
    return "TOK_GT";
  case TOK_NEQ:
    return "TOK_NEQ";
  case TOK_LTE:
    return "TOK_LTE";
  case TOK_GTE:
    return "TOK_GTE";
  case TOK_LPAREN:
    return "TOK_LPAREN";
  case TOK_RPAREN:
    return "TOK_RPAREN";
  case TOK_LBRACE:
    return "TOK_LBRACE";
  case TOK_RBRACE:
    return "TOK_RBRACE";
  case TOK_SEMI:
    return "TOK_SEMI";
  case TOK_COMMA:
    return "TOK_COMMA";
  case TOK_EOF:
    return "TOK_EOF";
  default:
    return "UNKNOWN_TOKEN";
  }
}
