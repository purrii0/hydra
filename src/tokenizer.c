#include "../include/tokenizer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_keyword(const char *s, TokenType *type) {
  if (strcmp(s, "int") == 0) {
    *type = TOK_INT;
    return 1;
  }
  if (strcmp(s, "char") == 0) {
    *type = TOK_CHAR;
    return 1;
  }
  if (strcmp(s, "float") == 0) {
    *type = TOK_FLOAT;
    return 1;
  }
  if (strcmp(s, "string") == 0) {
    *type = TOK_STRING;
    return 1;
  }
  if (strcmp(s, "void") == 0) {
    *type = TOK_VOID;
    return 1;
  }
  if (strcmp(s, "if") == 0) {
    *type = TOK_IF;
    return 1;
  }
  if (strcmp(s, "else") == 0) {
    *type = TOK_ELSE;
    return 1;
  }
  if (strcmp(s, "while") == 0) {
    *type = TOK_WHILE;
    return 1;
  }
  if (strcmp(s, "for") == 0) {
    *type = TOK_FOR;
    return 1;
  }
  if (strcmp(s, "return") == 0) {
    *type = TOK_RETURN;
    return 1;
  }
  return 0;
}

Token *tokenizer(const char *source) {
  Token *tokens = malloc(sizeof(Token) * 1024);
  int line = 1, pos = 0;
  const char *p = source;

  while (*p) {
    while (*p && isspace(*p)) {
      if (*p == '\n')
        line++;
      p++;
    }
    if (!*p)
      break;

    Token t;
    t.line = line;

    // identifiers / keywords
    if (isalpha(*p) || *p == '_') {
      char buf[256];
      int i = 0;
      while (isalnum(*p) || *p == '_')
        buf[i++] = *p++;
      buf[i] = 0;
      TokenType kt;
      if (is_keyword(buf, &kt)) {
        t.type = kt;
      } else {
        t.type = TOK_IDENT;
      }
      strcpy(t.lexeme, buf);
    }
    // integer literal
    else if (isdigit(*p)) {
      char buf[256];
      int i = 0;
      while (isdigit(*p))
        buf[i++] = *p++;
      buf[i] = 0;
      t.type = TOK_INT_LIT;
      strcpy(t.lexeme, buf);
    }
    // operators / punctuation
    else {
      switch (*p) {
      case '+':
        t.type = TOK_PLUS;
        t.lexeme[0] = '+';
        t.lexeme[1] = 0;
        p++;
        break;
      case '-':
        t.type = TOK_MINUS;
        t.lexeme[0] = '-';
        t.lexeme[1] = 0;
        p++;
        break;
      case '*':
        t.type = TOK_STAR;
        t.lexeme[0] = '*';
        t.lexeme[1] = 0;
        p++;
        break;
      case '/':
        t.type = TOK_SLASH;
        t.lexeme[0] = '/';
        t.lexeme[1] = 0;
        p++;
        break;
      case '=':
        if (*(p + 1) == '=') {
          t.type = TOK_EQEQ;
          strcpy(t.lexeme, "==");
          p += 2;
        } else {
          t.type = TOK_EQ;
          strcpy(t.lexeme, "=");
          p++;
        }
        break;
      case '<':
        if (*(p + 1) == '=') {
          t.type = TOK_LTE;
          strcpy(t.lexeme, "<=");
          p += 2;
        } else {
          t.type = TOK_LT;
          strcpy(t.lexeme, "<");
          p++;
        }
        break;
      case '>':
        if (*(p + 1) == '=') {
          t.type = TOK_GTE;
          strcpy(t.lexeme, ">=");
          p += 2;
        } else {
          t.type = TOK_GT;
          strcpy(t.lexeme, ">");
          p++;
        }
        break;
      case '!':
        if (*(p + 1) == '=') {
          t.type = TOK_NEQ;
          strcpy(t.lexeme, "!=");
          p += 2;
        } else {
          printf("Unexpected '!'\n");
          exit(1);
        }
        break;
      case '(':
        t.type = TOK_LPAREN;
        strcpy(t.lexeme, "(");
        p++;
        break;
      case ')':
        t.type = TOK_RPAREN;
        strcpy(t.lexeme, ")");
        p++;
        break;
      case '{':
        t.type = TOK_LBRACE;
        strcpy(t.lexeme, "{");
        p++;
        break;
      case '}':
        t.type = TOK_RBRACE;
        strcpy(t.lexeme, "}");
        p++;
        break;
      case ';':
        t.type = TOK_SEMI;
        strcpy(t.lexeme, ";");
        p++;
        break;
      case ',':
        t.type = TOK_COMMA;
        strcpy(t.lexeme, ",");
        p++;
        break;
      default:
        printf("Unknown char '%c'\n", *p);
        exit(1);
      }
    }

    tokens[pos++] = t;
  }

  tokens[pos].type = TOK_EOF;
  strcpy(tokens[pos].lexeme, "EOF");
  tokens[pos].line = line;

  return tokens;
}

