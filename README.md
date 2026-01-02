# Hydra Compiler

Hydra is a small **C-like compiler** written in **C**.
It takes source code written in a custom language, builds an **AST**, and **generates equivalent C code**.

This project is meant for learning **real compiler construction**, not interpretation.

---

## Features

- C-like syntax
- Functions
- Variable declarations with initialization
- Arithmetic expressions (`+ - * /`)
- Comparisons (`> < >= <= == !=`)
- `if / else`
- `return`
- Built-in `print()` function
- AST-based C code generation
- Makefile-based build system

---

## Example

### Input (`tst.hy`)

```c
int main() {
    int x = 5;
    int y = 10;
    int sum = x + y;

    print(sum);

    if (sum > 10) {
        return 1;
    }

    return 0;
}
```

---

### Compile with Hydra

```sh
./hydra tst.hy
```

This generates `out.c`.

---

### Generated C (`out.c`)

```c
#include <stdio.h>

int main() {
    int x = 5;
    int y = 10;
    int sum = (x + y);

    printf("%d\n", sum);

    if ((sum > 10)) {
        return 1;
    }

    return 0;
}
```

---

### Compile and run generated C

```sh
gcc out.c -o out
./out
```

---

## Built-in Functions

### `print(expr);`

Prints an integer expression followed by a newline.

```c
print(x);
print(x + 10);
```

Currently supports:

- integers

(Planned: floats, strings)

---

## Build Instructions

This project uses a **Makefile**.

### Build

```sh
make
```

or

```sh
make build
```

Produces the `hydra` compiler binary.

---

### Clean

```sh
make clean
```

Removes all build artifacts.

---

## Project Structure

```
hydra/
├── include/
│   ├── tokenizer.h
│   └── parser.h
├── src/
│   ├── tokenizer.c
│   ├── parser.c
│   └── codegen.c
├── main.c
├── Makefile
└── README.md
```

---

## How It Works

1. **Lexer** converts source code into tokens
2. **Parser** builds an Abstract Syntax Tree (AST)
3. **Code generator** walks the AST and emits C code
4. The generated C code is compiled using `gcc`

---

## Goals

- Learn real compiler internals
- Understand parsing and ASTs
- Build a foundation for LLVM or assembly backends

---

## Future Work

- Symbol table
- Type checking
- Function calls
- Loops (`while`, `for`)
- String and float printing
- LLVM IR backend

