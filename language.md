# Language Specification (`language.md`)

## 1. General Rules

* Every statement **must end with a semicolon (`;`)**.
* The program execution **starts from the `main` function**.
* Code blocks are enclosed using `{ }`.
* The language is **case-sensitive**.
* Whitespace is ignored except where it separates tokens.
* Valid variable names [a-zA-Z]\S* 
---

## 2. Data Types

| Type     | Description            |
| -------- | ---------------------- |
| `int`    | Integer numbers        |
| `char`   | Single character       |
| `string` | Sequence of characters |
| `float`  | Floating-point numbers |
| `void`   | No return value        |

---

## 3. Variables

### Declaration Syntax

```c
type variable_name;
type variable_name(value);
type variable_name{value};
type variable_name = value;
```

### Examples

```c
int count;
int age = 25;
int age(25);
float price = 10.5;
char letter = 'a';
string name = "John";
```

---

## 4. Functions

### Function Definition Syntax

```c
return_type function_name(arguments) {
    // statements
}
```

### Example

```c
int add(int a, int b) {
    return a + b;
}
```

---

## 5. Main Function

* The `main` function is the **entry point** of the program.
* It must return an `int`.

### Example

```c
int main() {
    return 0;
}
```

---

## 6. Return Statement

* Functions with a return type other than `void` **must** return a value.
* `void` functions may use `return;` or omit it.

### Examples

```c
int getNumber() {
    return 10;
}

void printHello() {
    return;
}
```

---

## 7. Control Flow

### If Statement

```c
if (condition) {
    // statements
}
```

```c
if (condition) {
    // statements
} else {
    // statements
}
```

### Example

```c
if (age > 18) {
    return 1;
} else {
    return 0;
}
```

---

### While Loop

```c
while (condition) {
    // statements
}
```

### Example

```c
int i = 0;
while (i < 5) {
    i = i + 1;
}
```

---

### For Loop

```c
for (initialization; condition; update) {
    // statements
}
```

### Example

```c
for (int i = 0; i < 10; i = i + 1) {
    // loop body
}
```

---

## 8. Operators

### Arithmetic Operators

* `+` addition
* `-` subtraction
* `*` multiplication
* `/` division

### Comparison Operators

* `==` equal
* `!=` not equal
* `<` less than
* `>` greater than
* `<=` less than or equal
* `>=` greater than or equal

---

## 9. Comments

### Single-line Comment

```c
// This is a comment
```

---

## 10. Example Program

```c
int main() {
    int x = 5;
    int y = 10;
    int sum = x + y;

    if (sum > 10) {
        return 1;
    }

    return 0;
}
```