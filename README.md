# Compiler Project - Group 24

## Overview

This is a complete **two-stage compiler** for a mini-language that processes a single `program.txt` file through:

1. **Lexer** (Lexical Analysis) → Generates tokens
2. **Parser** (Syntax Analysis) → Generates parse tree

## ⚡ Quick Start (30 seconds)

```bash
# Navigate to project directory
cd /path/to/Compiler-project---Group-24

# Run the unified pipeline
./compile.sh
```

The following will be displayed on the terminal:
-  Your source program (program.txt) 
-  Lexer output (tokens)
-  Parser output (parse tree)

---

## Project Structure

```
Compiler-project---Group-24/
├── program.txt           ← EDIT THIS: Your input program
├── compile.sh            ← RUN THIS: Unified pipeline script
├── QUICK_START.md        ← Start here for examples
├── UNIFIED_PIPELINE.md   ← Full pipeline documentation
├── WORKPLAN.md           ← Project plan with assignments
├── PROJECT_STATUS.md     ← Completion status
│
├── Lexer-G24/            ← Lexical analyzer component
│   ├── lexer.c
│   ├── lexer            (binary - auto-compiled)
│   ├── makefile
│   ├── readme.md
│   └── program.txt      (copy of main program.txt)
│
└── Parser-G24/          ← Syntax analyzer component
    ├── parser.c
    ├── parser.c         (binary - auto-compiled)
    ├── parse_tree.c
    ├── parse_tree.h
    ├── main.c
    ├── makefile
    ├── readme.md
    └── program.txt      (copy of main program.txt)
```

---

## The Pipeline

```
program.txt (YOUR INPUT)
    ↓
    ├─→ [LEXER] → Tokenizes
    │   Output: KEYWORD, IDENTIFIER, NUMBER, OPERATOR
    │
    └─→ [PARSER] → Builds Parse Tree
        Output: AST with grammar nodes
```

---

## Example Usage

### Example 1: Simple Program
```bash
printf "int x\nx = 10\n" > program.txt
./compile.sh
```

### Example 2: With Control Flow
```bash
printf "int count\ncount = 5\nwhile count\ncount = count + 1\n" > program.txt
./compile.sh
```

### Example 3: All Features
```bash
printf "int a\nint b\na = 10\nb = 20\nif a\na = a + b\n" > program.txt
./compile.sh
```

---

## Features

### Lexer (Lexer-G24)
- Scans mini-language source code
- Generates tokens:
  - **Keywords**: `int`, `char`, `if`, `while`, `for`
  - **Identifiers**: variable names
  - **Numbers**: numeric constants
  - **Operators**: `=`, `+`, `*`, `%`

### Parser (Parser-G24)
- Recursive-descent LL(1) parser
- Generates Abstract Syntax Tree (AST)
- Full grammar coverage:
  - Declarations
  - Assignments
  - Expressions (with operator precedence)
  - Control structures (if, while, for)

### Pipeline Script
- Auto-compiles components if needed
- Copies input to both components
- Displays outputs separately
- Color-coded for readability

---

## Grammar Supported

```
Program     → Statements
Statement   → Declaration | Assignment | IfStatement | WhileStatement | ForStatement
Declaration → int IDENTIFIER | char IDENTIFIER  
Assignment  → IDENTIFIER = Expression
IfStatement → if Expression Statement
WhileStatement → while Expression Statement
ForStatement → for Assignment Statement
Expression  → Term ((+) Term)*
Term        → Factor ((*, %) Factor)*
Factor      → IDENTIFIER | NUMBER
```

---

## Sample Output

### Lexer Output
```
KEYWORD: int
IDENTIFIER: x
IDENTIFIER: x
OPERATOR: =
NUMBER: 10
KEYWORD: if
IDENTIFIER: x
IDENTIFIER: x
OPERATOR: =
IDENTIFIER: x
OPERATOR: +
NUMBER: 5
```

### Parser Output
```
├─ Program
  ├─ Statements
    ├─ Statement
      ├─ Declaration
        ├─ Keyword : "int"
        ├─ Identifier : "x"
    ├─ Statements
      ├─ Statement
        ├─ Assignment
          ├─ Identifier : "x"
          ├─ Operator : "="
          ├─ Expression
            ├─ Term
              ├─ Factor
                ├─ Number : "10"
              ├─ Term
                ├─ ε
            ├─ Term
              ├─ ε
      ├─ Statements
        ├─ Statement
          ├─ IfStatement
            ├─ Keyword : "if"
            ├─ Expression
            ...
```

---

## Documentation

| Document | Purpose |
|---|---|
| [QUICK_START.md](QUICK_START.md) | Tutorial and examples |
| [UNIFIED_PIPELINE.md](UNIFIED_PIPELINE.md) | Pipeline architecture |
| [WORKPLAN.md](WORKPLAN.md) | Project plan with task breakdown |
| [PROJECT_STATUS.md](PROJECT_STATUS.md) | Completion report |
| Lexer-G24/readme.md | Lexer documentation |
| Parser-G24/readme.md | Parser documentation |

---

## Team & Roles

| Member | Role | Assignment |
|---|---|---|
| ODHIAMBO JOHN OTIENO | Token Integration | Lexer interface, error handling |
| MICHAEL MUTETI | Core Implementation | Parser initialization, testing |
| ABBA BIKO WERE | Parsing Logic | Statement/expression parsing |
| MARK WAFULA | Output Generation | Parse tree structure and printing |

---

## Getting Started

### Step 1: Verify Setup
```bash
ls -la Lexer-G24/
ls -la Parser-G24/
ls -la compile.sh
```

### Step 2: Create Your Program
```bash
# Option A: Text editor
nano program.txt

# Option B: Command line
echo "int x" > program.txt
echo "x = 5" >> program.txt
```

### Step 3: Run the Pipeline
```bash
./compile.sh
```

### Step 4: View Results
- **Source program** at top
- **Lexer tokens** in middle section
- **Parse tree** at bottom

---

## Troubleshooting

### Script not executable
```bash
chmod +x compile.sh
./compile.sh
```

### Components won't compile
```bash
# Manual compilation
make -C Lexer-G24
make -C Parser-G24
```

### Parse errors
- Check syntax against supported grammar
- Ensure variables declared before use (not required but helps)
- Verify keywords spelled correctly

---

## For Demonstration

### Demo Checklist
- [x] Source: `cat program.txt`
- [x] Run: `./compile.sh`
- [x] Lexer Output: Token stream shown
- [x] Parser Output: Parse tree shown
- [x] Explain: Each member covers their component

### What Assessors Will See
1. Input program (any valid mini-language code)
2. Lexer output (list of tokens)
3. Parser output (parse tree showing grammar)
4. All in one automated execution

---

## Valid Language Constructs

```
/* Declarations */
int variable_name
char variable_name

/* Assignments */
variable_name = expression
variable_name = 5
variable_name = other_variable
variable_name = x + 5
variable_name = x * 2
variable_name = x + 5 * 2

/* Conditionals */
if expression
  statement

/* Loops */
while expression
  statement

for variable = expression
  statement
```

---

## Project Status

**Status**: **COMPLETE**

- [x] Lexer implementation
- [x] Parser implementation with parse tree
- [x] Unified pipeline script
- [x] Complete documentation
- [x] Sample test programs
- [x] Workplan with assignments
- [x] Ready for live demo

---

## Quick Help

For quick answers, see:
- **How to run?** → [QUICK_START.md](QUICK_START.md)
- **How does it work?** → [UNIFIED_PIPELINE.md](UNIFIED_PIPELINE.md)
- **Who did what?** → [WORKPLAN.md](WORKPLAN.md)
- **Are we done?** → [PROJECT_STATUS.md](PROJECT_STATUS.md)

---

## Ready for April 24, 2026 Demo

All components tested and working. Ready for live demonstration with any sample input!

---

**Last Updated**: April 15, 2026  
**Project Status**: COMPLETE  
**Next Step**: Live demonstration
