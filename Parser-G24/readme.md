# LL(1) Parser for Mini-Grammar

This is a **table-driven LL(1) Parser** implementing recursive descent parsing for the mini-language tokens produced by the lexer.

## Grammar Definition

The parser uses the following inferred grammar optimized for LL(1) parsing:

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

## Grammar Analysis

### FIRST Sets
| Non-terminal | FIRST |
|---|---|
| Program | {int, char, IDENTIFIER, if, while, for} |
| Statements | {int, char, IDENTIFIER, if, while, for, ε} |
| Statement | {int, char, IDENTIFIER, if, while, for} |
| Declaration | {int, char} |
| Assignment | {IDENTIFIER} |
| IfStatement | {if} |
| WhileStatement | {while} |
| ForStatement | {for} |
| Expression | {IDENTIFIER, NUMBER} |
| AddExpr | {+, ε} |
| Term | {IDENTIFIER, NUMBER} |
| MulExpr | {*, %, ε} |
| Factor | {IDENTIFIER, NUMBER} |

### FOLLOW Sets
| Non-terminal | FOLLOW |
|---|---|
| Program | {EOF} |
| Statements | {EOF} |
| Statement | {int, char, IDENTIFIER, if, while, for, EOF} |
| Declaration | {int, char, IDENTIFIER, if, while, for, EOF} |
| Assignment | {int, char, IDENTIFIER, if, while, for, EOF} |
| IfStatement | {int, char, IDENTIFIER, if, while, for, EOF} |
| WhileStatement | {int, char, IDENTIFIER, if, while, for, EOF} |
| ForStatement | {int, char, IDENTIFIER, if, while, for, EOF} |
| Expression | {int, char, IDENTIFIER, if, while, for, EOF} |
| AddExpr | {int, char, IDENTIFIER, if, while, for, EOF} |
| Term | {+, int, char, IDENTIFIER, if, while, for, EOF} |
| MulExpr | {+, int, char, IDENTIFIER, if, while, for, EOF} |
| Factor | {*, %, +, int, char, IDENTIFIER, if, while, for, EOF} |

## LL(1) Property

This grammar is LL(1) because for each non-terminal with multiple productions, the FIRST sets of the alternatives are disjoint, ensuring deterministic parsing with a single lookahead token.

## Implementation Approach

The parser is implemented using **recursive descent** parsing with each grammar rule mapped to a function. While the current implementation uses a procedural approach rather than an explicit parsing table, the design ensures LL(1) properties through:

1. **Deterministic choices**: Each parsing function makes its decision based on the current token's type
2. **No backtracking**: Once a production is selected, it commits to that path
3. **Predictive parsing**: FIRST sets guide the parsing decisions

## Files

- **parser.h**: Header with token definitions and data structures
- **parser.c**: Core parser implementation with recursive descent functions
- **main.c**: Driver program that reads from `program.txt` and parses
- **makefile**: Build configuration

## Compilation

```bash
make
```

## Running the Parser

```bash
make run
```

The parser reads input from `program.txt` in the Parser-G24 directory.

## Example Input

```
int count
count = 5 + 3
while count
  count = count * 2
```

## Example Output

```
=== LL(1) Parser Starting ===

Parsing Program
Parsing Statements
Parsing Statement
Parsing Declaration
  TYPE: int
  IDENTIFIER: count
Parsing Statements
Parsing Statement
Parsing Assignment
  IDENTIFIER: count
  OPERATOR: =
Parsing Expression
Parsing Term
Parsing Factor
  NUMBER: 5
Parsing MulExpr
Parsing AddExpr
  OPERATOR: +
Parsing Term
Parsing Factor
  NUMBER: 3
Parsing MulExpr
Parsing AddExpr
...

=== Parsing Successful ===
```

## Parsing Strategy

### Program Analysis Flow

1. **Lexical Analysis**: The parser reads tokens from the input file character by character
2. **Syntax Analysis**: Using the grammar rules and FIRST/FOLLOW sets, the parser recursively processes tokens
3. **Parsing Tree Construction**: Each function call represents a level in the parse tree

### Error Handling

The parser provides basic error messages for:
- Unexpected tokens
- Missing keywords or operators
- Invalid syntax

## Production Rules Summary

| # | Rule | Type |
|---|---|---|
| 0 | Program → Statements | Start |
| 1 | Statements → Statement Statements | Recursive |
| 2 | Statements → ε | Base |
| 3-7 | Statement alternatives | Disambiguation |
| 8-9 | Declaration (int/char) | Terminal |
| 10 | Assignment | Terminal |
| 11-13 | Control Structures | Terminal |
| 14 | Expression → Term AddExpr | Binary |
| 15 | AddExpr → + Term AddExpr | Right-recursive |
| 16 | AddExpr → ε | Base |
| 17 | Term → Factor MulExpr | Binary |
| 18-19 | MulExpr (*,%) | Right-recursive |
| 20 | MulExpr → ε | Base |
| 21-22 | Factor terminals | Terminal |

## Authors

Group 24 – Compiler Construction

     ODHIAMBO JOHN OTIENO - SCS3/147357/2023
     MICHAEL MUTETI - SCS3/147532/2024
     ABBA BIKO WERE - SCS3/146750/2023
     MARK WAFULA - SCS3/146670/2023
