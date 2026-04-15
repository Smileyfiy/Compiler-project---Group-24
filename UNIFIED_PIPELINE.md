# Unified Compiler Pipeline

This directory contains a complete compiler pipeline that processes a single `program.txt` file through both the **Lexer** and **Parser** components.

## Overview

The unified workflow:
1. **Input**: Single `program.txt` file with mini-language source code
2. **Lexical Analysis**: Lexer-G24 tokenizes the input
3. **Syntax Analysis**: Parser-G24 parses tokens and generates parse tree
4. **Output**: Displays both lexer tokens and parser tree separately

## Quick Start

```bash
# Run the complete pipeline
./compile.sh
```

## File Structure

```
Compiler-project---Group-24/
├── program.txt              ← Main input file (edit this)
├── compile.sh               ← Run this script
├── Lexer-G24/              ← Lexical analyzer
│   ├── lexer.c
│   ├── lexer (binary)
│   └── program.txt (copy)
└── Parser-G24/             ← Syntax analyzer
    ├── parser.c
    ├── parse_tree.c
    ├── parser (binary)
    └── program.txt (copy)
```

## Workflow

```
program.txt (source code)
    ↓
    └→ [Lexer-G24] → Token Stream
                        ↓
                    [Parser-G24] → Parse Tree (AST)
```

## Step-by-Step Output

### Step 1: Source Program
Displays the input mini-language program

### Step 2: Lexical Analysis
Shows tokens from lexer:
```
KEYWORD: int
IDENTIFIER: x
OPERATOR: =
NUMBER: 10
...
```

### Step 3: Syntax Analysis  
Shows parse tree from parser:
```
├─ Program
  ├─ Statements
    ├─ Statement
      ├─ Declaration
        ├─ Keyword : "int"
        ├─ Identifier : "x"
    ...
```

---

## Example Programs

### Simple Declaration and Assignment
```
int x
x = 5
```

### With If Statement
```
int x
x = 10
if x
x = x + 5
```

### With While Loop
```
int count
count = 10
while count
count = count + 1
```

### Complex Program
```
int count
int sum
count = 5
sum = 0
while count
sum = sum + count
count = count + 1
if sum
count = 10
```

---

## Editing Input Program

Simply edit `program.txt` with any valid mini-language code:

```bash
# Edit the input file
nano program.txt

# Or use printf/echo
printf "int x\nx = 5 + 3\n" > program.txt

# Then run the pipeline
./compile.sh
```

## Manual Compilation

If needed, compile components separately:

```bash
# Compile only lexer
make -C Lexer-G24

# Compile only parser  
make -C Parser-G24

# or both
make -C Lexer-G24 && make -C Parser-G24
```

## Clean Up

```bash
# Remove compiled binaries
make -C Lexer-G24 clean
make -C Parser-G24 clean

# Or keep the script
./compile.sh  # will recompile if needed
```

---

## Output Interpretation

### Lexer Output
```
TOKEN_TYPE: token_value
```

Example:
```
KEYWORD: if          ← if keyword
IDENTIFIER: count    ← variable name
OPERATOR: =          ← assignment
NUMBER: 10           ← numeric literal
```

### Parser Output
```
Program
├─ Grammar Rule Name
  ├─ Child Node : "token_value"
  └─ ...
```

The tree shows:
- **Grammar rules** applied during parsing
- **Token values** like variable names and numbers
- **Node types** for semantic information
- **Tree structure** showing parse relationships

---

## Troubleshooting

### "program.txt not found"
```bash
cd Compiler-project---Group-24
# Ensure you're in the right directory
echo "int x" > program.txt
./compile.sh
```

### "Lexer/Parser not compiled"
The script auto-compiles if binaries are missing. Make sure:
- Lexer-G24/ and Parser-G24/ directories exist
- Source files (lexer.c, parser.c) are present

### Parse errors
Common issues:
- Missing semicolons (not required in this grammar)
- Variables not declared before use (not enforced)
- Syntax errors in expressions

Example error:
```
PARSE ERROR: Expected identifier in declaration
ERROR: Expected EOF, got identifier
```

---

## Workflow Details

### 1. Source Program Input
```
program.txt contains mini-language code
```

### 2. Lexer Process
- Reads characters from program.txt
- Groups into tokens (keywords, identifiers, numbers, operators)
- Outputs token stream

### 3. Parser Process
- Reads tokens from lexer output
- Matches against grammar rules
- Builds Abstract Syntax Tree (AST)
- Outputs parse tree with indentation

### 4. Overall Pipeline
```
Source Code → Lexer → Tokens → Parser → Parse Tree (AST)
```

---

## Automation Features

The `compile.sh` script:
-  Checks for program.txt existence
-  Verifies both projects exist
-  Auto-compiles if binaries missing
-  Copies program.txt to both directories
-  Runs lexer and shows output
-  Runs parser and shows output
-  Color-codes output for readability
-  Shows pipeline completion status

---

## Project Structure Reference

| Component | Purpose | Language |
|---|---|---|
| Lexer-G24 | Tokenization | C |
| Parser-G24 | Syntax analysis & tree generation | C |
| compile.sh | Orchestration | Bash |

---

## For Assessment

When demonstrating to assessors:

1. **Show the source**: `cat program.txt`
2. **Run the pipeline**: `./compile.sh`
3. **Explain lexer output**: Token types and values
4. **Explain parser output**: Grammar rules and parse tree structure

---

## Quick Examples

### Run with custom program
```bash
echo "int x
x = 5 * 2" > program.txt
./compile.sh
```

### Capture output to file
```bash
./compile.sh > output.txt 2>&1
cat output.txt
```

### Run only part of pipeline
```bash
# Just lexer
cd Lexer-G24 && ./lexer

# Just parser
cd Parser-G24 && ./parser
```

---

**Ready for live demonstration on April 24, 2026** 

Members can explain:
- JOHN: Lexer/Token integration  
- MICHAEL: Parser core and testing
- ABBA: Grammar rules in parse tree
- MARK: AST structure visualization
