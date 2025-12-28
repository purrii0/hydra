# Language Specification

## 1. Keywords

```text
int
float
char
string
void
if
else
while
for
return
main
```

---

## 2. Tokens

### 2.1 Data Type Tokens

```text
TOK_INT
TOK_FLOAT
TOK_CHAR
TOK_STRING
TOK_VOID
```

### 2.2 Control Flow Tokens

```text
TOK_IF
TOK_ELSE
TOK_WHILE
TOK_FOR
TOK_RETURN
```

### 2.3 Program Structure Tokens

```text
TOK_MAIN
TOK_IDENT
```

### 2.4 Literal Tokens

```text
TOK_INT_LIT
TOK_FLOAT_LIT
TOK_CHAR_LIT
TOK_STRING_LIT
```

### 2.5 Operator Tokens

```text
TOK_PLUS     (+)
TOK_MINUS    (-)
TOK_STAR     (*)
TOK_SLASH    (/)

TOK_EQ       (=)
TOK_EQEQ     (==)
TOK_NEQ      (!=)
TOK_LT       (<)
TOK_GT       (>)
TOK_LTE      (<=)
TOK_GTE      (>=)
```

### 2.6 Delimiter Tokens

```text
TOK_LPAREN   (()
TOK_RPAREN   ())
TOK_LBRACE   ({)
TOK_RBRACE   (})
TOK_SEMI     (;)
TOK_COMMA    (,)
```

### 2.7 Special Token

```text
TOK_EOF
```

## 3. Parser

### Example Program

```c
int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}
```

### AST Tree

```
NODE_FUNC_DECL ("max")
├─ NODE_PARAM_LIST
│  ├─ NODE_PARAM ("a") [type="int"]
│  └─ NODE_PARAM ("b") [type="int"]
└─ NODE_BLOCK
   └─ NODE_IF
      ├─ NODE_BINARY_OP (>)
      │  ├─ NODE_IDENT ("a")
      │  └─ NODE_IDENT ("b")
      ├─ NODE_RETURN
      │  └─ NODE_IDENT ("a")
      └─ NODE_RETURN
         └─ NODE_IDENT ("b")
```

