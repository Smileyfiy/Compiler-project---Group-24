# Parser Error Messages Documentation

This document describes all possible error messages that the LL(1) Parser may generate, their causes, and how to fix them.

## Error Message Format

All parse errors follow this format:
```
PARSE ERROR [Line X]: <error message>
```

Where `X` is the line number where the error was detected.

---

## Error Messages Reference

### 1. Invalid statement
**Line:** Reported at the line containing the invalid statement

**Cause:** The parser encountered a token that does not start a valid statement.

**Expected tokens at statement start:**
- `int` or `char` (declaration)
- Identifier (assignment)
- `if` (if statement)
- `while` (while statement)
- `for` (for statement)

**Example triggering this error:**
```
int x
5 = 10    # Line 2: Literal numbers cannot start a statement
```

**Fix:** Ensure statements begin with valid keywords or identifiers.

---

### 2. Expected 'int' or 'char' in declaration
**Line:** Reported at the line with the declaration keyword error

**Cause:** A declaration statement was started but neither `int` nor `char` keyword was found as the type specifier.

**Example triggering this error:**
```
double x    # Line 1: 'double' is not a valid type keyword
```

**Fix:** Use only `int` or `char` as variable type declarations.

---

### 3. Expected identifier in declaration
**Line:** Reported at the line with the missing identifier

**Cause:** A type keyword (`int` or `char`) was found, but the variable name (identifier) is missing.

**Example triggering this error:**
```
int
int y
```

**Fix:** Provide a valid identifier (variable name) after the type keyword.

---

### 4. Expected identifier in assignment
**Line:** Reported at the line with the assignment error

**Cause:** An assignment statement was expected to start with an identifier, but none was found.

**Example triggering this error:**
```
= 10    # Line 1: Missing variable name before assignment operator
```

**Fix:** Start assignment statements with a variable identifier.

---

### 5. Expected '=' in assignment
**Line:** Reported at the line with the missing assignment operator

**Cause:** An identifier was found at the start of a statement, but the assignment operator `=` is missing.

**Example triggering this error:**
```
x 10    # Line 1: Missing '=' between variable and value
```

**Fix:** Use `=` to assign values to variables.

---

### 6. Expected 'if'
**Line:** Reported at the if statement

**Cause:** An `if` statement was detected but the keyword `if` is missing.

**Example triggering this error:**
```
# This error is rare as it's caught by the statement parser first
# It would occur in nested contexts where 'if' was expected
```

**Fix:** Ensure `if` keyword is present in if statements.

---

### 7. Expected 'while'
**Line:** Reported at the while statement

**Cause:** A `while` statement was started but the `while` keyword is missing.

**Example triggering this error:**
```
loop x
  x = x + 1
```

**Fix:** Use the `while` keyword to begin while loops.

---

### 8. Expected 'for'
**Line:** Reported at the for statement

**Cause:** A `for` statement was started but the `for` keyword is missing.

**Example triggering this error:**
```
loop x = 0
  x = x + 1
```

**Fix:** Use the `for` keyword to begin for loops.

---

### 9. Expected identifier or number in factor
**Line:** Reported at the expression/factor

**Cause:** An expression factor must be either a variable identifier or a numeric literal. Neither was found.

**Example triggering this error:**
```
int x
x = +    # Line 2: No valid factor after the + operator
```

**Fix:** Provide a valid identifier or number in expressions.

---

### 10. Expected EOF, got <token>
**Line:** Reported at the end of parsing

**Cause:** Unexpected tokens remain after the program should have ended. The parser expected end-of-file but found additional tokens.

**Example triggering this error:**
```
int x
x = 5
extra    # This creates unexpected trailing content
```

**Fix:** Ensure all input is valid program statements with no trailing tokens.

---

## Quick Reference Table

| Error Message | Typical Cause | Quick Fix |
|---|---|---|
| Invalid statement | Wrong statement start token | Use `int`, `char`, identifier, `if`, `while`, or `for` |
| Expected 'int' or 'char' in declaration | Wrong type keyword | Change to `int` or `char` |
| Expected identifier in declaration | Missing variable name | Add variable name after type |
| Expected identifier in assignment | Missing variable name | Start with a valid identifier |
| Expected '=' in assignment | Missing assignment operator | Add `=` after variable name |
| Expected 'if' | Missing if keyword | Use `if` keyword |
| Expected 'while' | Missing while keyword | Use `while` keyword |
| Expected 'for' | Missing for keyword | Use `for` keyword |
| Expected identifier or number in factor | Invalid expression | Use identifier or number literal |
| Expected EOF, got [token] | Extra tokens at end | Remove trailing tokens |

---

## Valid Program Structure

### Declaration
```
int <identifier>
char <identifier>
```

### Assignment
```
<identifier> = <expression>
```

### If Statement
```
if <expression>
  <statement>
```

### While Loop
```
while <expression>
  <statement>
```

### For Loop
```
for <identifier> = <expression>
  <statement>
```

### Expression
```
<term> [+ <term>]*
```

### Term
```
<factor> [* <factor> | % <factor>]*
```

### Factor
```
<identifier> | <number>
```

---

## Example: Valid Program

```
int x
int y
x = 5
y = 10
if x
  y = y + 2
while y
  y = y * 2
for x = 1
  y = y + 1
```

This program declares two variables, assigns values, and uses control flow statements.

---

## Example: Program with Errors

```
1: int x
2: int          # ERROR: Expected identifier in declaration
3: x = 5
4: = 10         # ERROR: Expected identifier in assignment
5: if x
6:   y = *      # ERROR: Expected identifier or number in factor
```

Output:
```
PARSE ERROR [Line 2]: Expected identifier in declaration
PARSE ERROR [Line 4]: Expected identifier in assignment
PARSE ERROR [Line 6]: Expected identifier or number in factor
```

---

## Line Number Tracking

The parser automatically tracks line numbers by counting newline characters (`\n`) encountered during tokenization. This ensures accurate error reporting for debugging purposes.

- Line counting starts at 1
- Each newline character increments the line counter
- Error line numbers refer to where the problematic token was found
