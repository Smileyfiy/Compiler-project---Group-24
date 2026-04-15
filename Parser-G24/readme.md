# LL(1) Parser for Mini-Grammar with Parse Tree Generation

## Overview

This is a **recursive-descent LL(1) Parser** that generates **Abstract Syntax Trees (parse trees)** for the mini-language tokens produced by the lexer. The parser implements the decision taken by Group 24 to use a **Recursive-Descent Predictive Parser** approach.

## Implementation Approach Selected

**Decision: Recursive-Descent Predictive LL(1) Parser**

**Rationale:**
- LL(1) properties ensure deterministic parsing with single lookahead
- Recursive descent is intuitive, easy to debug, and maintain
- Suitable for the mini-grammar complexity (non-ambiguous)
- Direct integration with lexer tokens without intermediate buffering
- Clear, detailed error reporting at grammar rule level
- Natural parse tree generation through function call stack
- Easier to explain and demonstrate for live presentation

---

## Grammar Definition

```
Program     → Statements
Statements  → Statement Statements | ε
Statement   → Declaration | Assignment | IfStatement | WhileStatement | ForStatement

Declaration → int IDENTIFIER | char IDENTIFIER
Assignment  → IDENTIFIER = Expression
IfStatement → if Expression Statement
WhileStatement → while Expression Statement
ForStatement → for Assignment Statement

Expression  → Term AddExpr
AddExpr     → + Term AddExpr | ε
Term        → Factor MulExpr
MulExpr     → * Factor MulExpr | % Factor MulExpr | ε
Factor      → IDENTIFIER | NUMBER
```

### Grammar Analysis

#### FIRST Sets
| Non-terminal | FIRST |
|---|---|
| Program | {int, char, IDENTIFIER, if, while, for} |
| Statements | {int, char, IDENTIFIER, if, while, for, ε} |
| Statement | {int, char, IDENTIFIER, if, while, for} |
| Expression | {IDENTIFIER, NUMBER} |
| Term | {IDENTIFIER, NUMBER} |
| Factor | {IDENTIFIER, NUMBER} |

#### FOLLOW Sets
| Non-terminal | FOLLOW |
|---|---|
| Program | {EOF} |
| Statements | {EOF} |
| Statement | {int, char, IDENTIFIER, if, while, for, EOF} |
| Expression | {keywords, identifiers, EOF} |

### LL(1) Property Verification

This grammar is **LL(1)** because:
- For each non-terminal with multiple productions, FIRST sets are **disjoint**
- No conflicts in parsing decisions
- Single lookahead token suffices for deterministic parsing

---

## Key Features

✅ **Parse Tree Generation**: Builds Abstract Syntax Tree (AST) for successful parses  
✅ **Token Integration**: Reads tokens directly from input file  
✅ **Detailed Error Messages**: Clear error reporting for parse failures  
✅ **Tree Visualization**: Indented parse tree output showing grammar rules  
✅ **Node Types**: Semantic information preserved in AST nodes  
✅ **Memory Management**: Proper allocation and deallocation of tree nodes  

---

## File Structure

```
Parser-G24/
├── parser.h              # Token types and parser interface
├── parser.c              # Recursive descent parser implementation
├── parse_tree.h          # AST node data structures
├── parse_tree.c          # Parse tree manipulation functions
├── main.c                # Driver program
├── makefile              # Build configuration
├── readme.md             # This file
├── program.txt           # Input file (parsed program)
├── run_samples.sh        # Sample test suite
└── samples/
    ├── sample1_declarations.txt
    ├── sample2_expressions.txt
    ├── sample3_if_statement.txt
    ├── sample4_while_loop.txt
    ├── sample5_for_loop.txt
    └── sample6_complex.txt
```

---

## Compilation

```bash
cd Parser-G24
make          # Compile all sources
make clean    # Remove generated files
```

**Output**: Executable `parser` file

---

## Running the Parser

### Basic Usage
```bash
./parser
```
Parses `program.txt` and outputs parse tree

### Run All Sample Tests
```bash
./run_samples.sh
```
Executes parser on all sample test cases showing input and output

### With Specific Test File
```bash
cp samples/sample2_expressions.txt program.txt
./parser
```

---

## Parse Tree Output Example

### Input Program
```
int x
x = 5 + 3
```

### Parse Tree Output
```
=== PARSE TREE ===

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
                ├─ Number : "5"
          ... [continues for right operand]
      ├─ Statements
        ├─ ε
```

---

## AST Node Types

| Node Type | Description |
|---|---|
| NODE_PROGRAM | Root of parse tree |
| NODE_STATEMENTS | Statement sequence |
| NODE_STATEMENT | Single statement |
| NODE_DECLARATION | Variable declaration |
| NODE_ASSIGNMENT | Variable assignment |
| NODE_IF_STATEMENT | If control structure |
| NODE_WHILE_STATEMENT | While control structure |
| NODE_FOR_STATEMENT | For loop structure |
| NODE_EXPRESSION | Arithmetic expression |
| NODE_TERM | Multiplicative subexpression |
| NODE_FACTOR | Atomic value |
| NODE_OPERATOR | Binary operator (+, *, %, =) |
| NODE_IDENTIFIER | Variable name |
| NODE_NUMBER | Numeric literal |
| NODE_KEYWORD | Language keyword |
| NODE_EPSILON | Empty production |

---

## Error Handling

The parser detects and reports:
- **Syntax Errors**: Missing or unexpected tokens
- **Type Errors**: Wrong token type in context
- **Unexpected EOF**: Incomplete programs

**Example Error Message**:
```
PARSE ERROR: Expected identifier in declaration
ERROR: Expected EOF, got identifier
```

---

## Testing

### Sample Test Cases
1. **Declarations**: Simple int/char variable declarations
2. **Expressions**: Arithmetic expressions with operators  
3. **If Statements**: Conditional execution
4. **While Loops**: Iterative execution
5. **For Loops**: Counter-based iteration
6. **Complex**: Combined features with nesting

**Run Tests**: Execute `./run_samples.sh` to test all cases

---

## Parsing Algorithm

### Recursive Descent Steps

1. **Initialization**: Read first token from input
2. **Grammar Rule Processing**:
   - Each function implements one grammar rule
   - FIRST set determines which production to use
   - Consume matching tokens and recurse
3. **Node Creation**: Build AST node for each rule matched
4. **Tree Linking**: Connect parent and child nodes
5. **Validation**: Check for EOF at end of parse

### For Each Production:
```
parse_rule:
1. Create AST node for this rule
2. Check lookahead token (FIRST set)
3. Recursively parse child rules
4. Add child nodes to parent
5. Return constructed subtree
```

---

## Workplan Adherence

This implementation follows the project **workplan** with:

 **Task Definition**: All parsing modules documented  
 **Effort Estimation**: 14.5 days distributed across 3 weeks  
 **Responsibility Assignment**: Clear task allocation to team members  
 **Implementation**: Recursive descent with full grammar coverage  
 **Document Deliverables**: Source code, token traces, parse trees  
 **Demo Preparation**: Sample programs ready for April 24 presentation  

---

## Live Demo Preparation

### Demo Flow
1. **Input**: Show mini-language source code
2. **Lexer Output**: Display tokenized output
3. **Parse Stage**: Run parser with tokens
4. **Parse Tree**: Show AST visualization
5. **Explanation**: Team members explain components

### Demo Materials Ready
-  Sample programs in `samples/` directory
-  Token trace examples
-  Parse tree outputs for each test case
-  Error handling demonstrations

---

## Member Roles (From Workplan)

| Name | Role | Responsibility |
|---|---|---|
| JOHN OTIENO | Token Integration & Error Handling | Lexer interface, error messages |
| MICHAEL MUTETI | Parser Core & Testing | Parser initialization, test cases |
| ABBA BIKO WERE | Parsing Functions | Statement and expression parsers |
| MARK WAFULA | Parse Tree & Output | AST structures, tree visualization |

---

## References

- **Grammar**: Mini-grammar for simple programming language
- **Implementation**: Recursive descent parsing
- **AST**: Standard abstract syntax tree representation
- **Standards**: C99/C11 with POSIX functions

---

## Building & Running

### Quick Start
```bash
cd Parser-G24
make
echo "int x\nx = 10" > program.txt
./parser
```

### Test Suite
```bash
./run_samples.sh | tee test_results.log
```

### Cleanup
```bash
make clean
```

---

**Last Updated**: April 15, 2026  
**Status**:  Parse Tree Generation Implemented  
**Next**: Integration test with lexer output

---

## Authors

Group 24 – Compiler Construction

     ODHIAMBO JOHN OTIENO - SCS3/147357/2023
     MICHAEL MUTETI - SCS3/147532/2024
     ABBA BIKO WERE - SCS3/146750/2023
     MARK WAFULA - SCS3/146670/2023

