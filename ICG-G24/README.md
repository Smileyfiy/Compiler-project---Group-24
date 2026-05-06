# Intermediate Code Generation (ICG) - Quadruples

## Overview

This ICG (Intermediate Code Generation) system generates **quadruples** as an intermediate representation from the Abstract Syntax Tree (AST) produced by the parser. Quadruples are a machine-independent, three-address code format suitable for further compilation stages.

## Architecture

```
Source Code (program.txt)
    ↓
[Lexer] → Token Stream
    ↓
[Parser] → AST (Abstract Syntax Tree)
    ↓
[ICG with Quadruples] → Intermediate Code
    ↓
(Ready for Code Generation / Optimization)
```

## Files

| File | Purpose |
|------|---------|
| `icg.h` | Header file - defines structures and function prototypes |
| `icg.c` | Core ICG implementation - AST traversal and quadruple generation |
| `icg_main.c` | Main driver - orchestrates the full compilation pipeline |
| `makefile` | Build configuration for ICG system |

## Building

```bash
# Compile the ICG system
make clean && make

# Run the ICG on a program
make run

# Or manually:
./icg ../program.txt output.txt
```

## Quadruple Format

A quadruple consists of four fields: **(OP, ARG1, ARG2, RESULT)**

### Operation Types

| Operation | Format | Example | Purpose |
|-----------|--------|---------|---------|
| **=** (ASSIGN) | (=, RHS, -, LHS) | (=, 5, -, x) | Variable assignment |
| **+** (ADD) | (+, arg1, arg2, result) | (+, x, y, t0) | Addition |
| **-** (SUB) | (-, arg1, arg2, result) | (-, a, b, t1) | Subtraction |
| **\*** (MUL) | (*, arg1, arg2, result) | (*, x, 2, t2) | Multiplication |
| **%** (MOD) | (%, arg1, arg2, result) | (%, n, 2, t3) | Modulo |
| **<** (LT) | (<, arg1, arg2, result) | (<, x, 10, t4) | Less than comparison |
| **>** (GT) | (>, arg1, arg2, result) | (>, x, 5, t5) | Greater than comparison |
| **label** | (label, label_name, -, -) | (label, L1, -, -) | Jump target |
| **goto** | (goto, label, -, -) | (goto, L2, -, -) | Unconditional jump |
| **if_goto** | (if_goto, condition, 0, label) | (if_goto, t5, 0, L2) | Conditional jump (jump if false) |
| **declare** | (declare, type, -, var_name) | (declare, int, -, x) | Variable declaration |
| **end** | (end, -, -, -) | (end, -, -, -) | Program termination |

**Note**: ICG only uses operators recognized by Lexer-G24: `=`, `+`, `-`, `*`, `%`, `<`, `>`

## Symbol Table

The ICG maintains a **symbol table** that tracks:
- **Variable name**: Identifier
- **Type**: Data type (int, char, etc.)
- **Scope level**: Lexical scope depth
- **Initialization**: Whether variable has been initialized

### Symbol Table Entry Structure
```c
typedef struct {
    char name[100];         /* Variable name */
    char type[50];          /* Type: int, char, etc. */
    int scope_level;        /* Scope depth (0 = global) */
    int initialized;        /* Initialization flag */
} Symbol;
```

## Core Data Structures

### Quadruple
```c
typedef struct {
    OperationType op;       /* Operation type */
    char arg1[100];        /* First argument */
    char arg2[100];        /* Second argument */
    char result[100];      /* Result/destination */
} Quadruple;
```

### ICG Context
```c
typedef struct {
    QuadrupleList *quad_list;      /* List of generated quadruples */
    SymbolTable *symbol_table;     /* Symbol table for variables */
    int temp_var_count;            /* Counter for t0, t1, t2, ... */
    int label_count;               /* Counter for L0, L1, L2, ... */
    int scope_level;               /* Current scope depth */
} ICGContext;
```

## Key Functions

### Initialization
```c
ICGContext* icg_init();              /* Initialize ICG context */
void icg_free(ICGContext *ctx);      /* Free ICG context */
```

### Code Generation
```c
void icg_generate(ParseTreeNode *ast, ICGContext *ctx);
void icg_emit_quad(ICGContext *ctx, OperationType op, 
                   const char *arg1, const char *arg2, const char *result);
```

### Symbol Table Management
```c
void symbol_table_insert(SymbolTable *st, const char *name, 
                         const char *type, int scope);
Symbol* symbol_table_lookup(SymbolTable *st, const char *name);
int symbol_table_exists(SymbolTable *st, const char *name);
```

### Temporary Variables & Labels
```c
char* icg_get_temp_var(ICGContext *ctx);   /* Generate t0, t1, ... */
char* icg_get_label(ICGContext *ctx);      /* Generate L0, L1, ... */
```

### Output
```c
void quad_list_print(QuadrupleList *ql, FILE *f);   /* Print quadruples */
```

## Example: Variable Declaration and Assignment

### Input Program
```
int x
x = 5
```

### Generated Quadruples
| NUM | OP | ARG1 | ARG2 | RESULT |
|-----|----|----|------|--------|
| 0 | declare | int | - | x |
| 1 | = | 5 | - | x |
| 2 | end | - | - | - |

### Symbol Table
| Variable | Type | Scope |
|----------|------|-------|
| x | int | 0 |

## Example: Arithmetic Expression

### Input Program
```
int x
int y
y = x + 5 * 2
```

### Generated Quadruples
| NUM | OP | ARG1 | ARG2 | RESULT |
|-----|----|----|------|--------|
| 0 | declare | int | - | x |
| 1 | declare | int | - | y |
| 2 | * | 5 | 2 | t0 |
| 3 | + | x | t0 | t1 |
| 4 | = | t1 | - | y |
| 5 | end | - | - | - |

## Example: Conditional Statement

### Input Program
```
if x > 3
y = 1
```

### Generated Quadruples
| NUM | OP | ARG1 | ARG2 | RESULT |
|-----|----|----|------|--------|
| 0 | declare | int | - | x |
| 1 | declare | int | - | y |
| 2 | > | x | 3 | t0 |
| 3 | if_goto | t0 | 0 | L0 |
| 4 | = | 1 | - | y |
| 5 | goto | L1 | - | - |
| 6 | label | L0 | - | - |
| 7 | label | L1 | - | - |
| 8 | end | - | - | - |

## Example: Loop Statement

### Input Program
```
while i < 10
x = x + 1
```

### Generated Quadruples
| NUM | OP | ARG1 | ARG2 | RESULT |
|-----|----|----|------|--------|
| 0 | declare | int | - | i |
| 1 | declare | int | - | x |
| 2 | label | L0 | - | - |
| 3 | < | i | 10 | t0 |
| 4 | if_goto | t0 | 0 | L1 |
| 5 | + | x | 1 | t1 |
| 6 | = | t1 | - | x |
| 7 | goto | L0 | - | - |
| 8 | label | L1 | - | - |
| 9 | end | - | - | - |

## Output Files

The ICG generates two output files:

1. **Console Output**: Display on terminal with formatted quadruple table
2. **icg_output.txt**: File containing:
   - Formatted quadruple table
   - Symbol table with all variables
   - Complete compilation log

## Usage

### Basic Usage
```bash
cd ICG-G24
./icg ../program.txt icg_output.txt
```

### Output Example
```
========== INTERMEDIATE CODE (QUADRUPLES) ==========
NUM   | OP              | ARG1            | ARG2            | RESULT         
------|-----------------|-----------------|-----------------|------------------
0     | declare         | int             | -               | x              
1     | declare         | int             | -               | y              
2     | =               | 5               | -               | x              
3     | =               | 10              | -               | y              
4     | *               | y               | 2               | t0             
5     | =               | t0              | -               | y              
6     | end             | -               | -               | -              

Symbol Table:
Variable             | Type       | Scope     
--------------------|-----------|----------
x                    | int        | 0         
y                    | int        | 0         

ICG completed successfully. Output written to: icg_output.txt
```

## Features

✅ **Quadruple Generation** - Three-address code format  
✅ **Symbol Table Management** - Track variable declarations and scope  
✅ **Arithmetic Expression Handling** - Binary operations with temporary variables  
✅ **Control Flow** - Labels and conditional/unconditional jumps for if/while loops  
✅ **Temporary Variable Generation** - Automatic t0, t1, t2, ... generation  
✅ **Label Generation** - Automatic L0, L1, L2, ... for control flow  
✅ **Formatted Output** - Both console and file output with proper tables  

## Integration with Compiler Pipeline

The ICG system is integrated into the complete compiler pipeline:

```bash
# Run complete pipeline
cd /path/to/Compiler-project---Group-24
./compile.sh
```

This will:
1. Run the Lexer → Tokens
2. Run the Parser → AST
3. Run the ICG → Quadruples
4. Display all outputs

## Next Steps

The generated quadruples can be used for:
- **Code Optimization**: Remove redundant operations, constant folding, etc.
- **Code Generation**: Translate to assembly language or machine code
- **Semantic Analysis**: Check for undefined variables, type mismatches
- **Register Allocation**: Assign temporary variables to CPU registers

## Team

Group 24 - Compiler Construction Project

---

**Note**: This ICG system works in conjunction with the Lexer and Parser components to form a complete compiler front-end.
