# ICG Implementation Summary

## Overview

Successfully implemented a complete **Intermediate Code Generation (ICG)** system using **quadruples** for the Group 24 Compiler Project. The ICG converts the Abstract Syntax Tree (AST) produced by the parser into an intermediate three-address code format suitable for further compilation stages like optimization and code generation.

## Components Implemented

### 1. **icg.h** - Header File
Defines all data structures and function prototypes:
- **Quadruple Operations**: 21 operation types (arithmetic, relational, control flow, etc.)
- **Symbol Table**: For tracking variable declarations and scope
- **Context Management**: Manages temporary variables, labels, and compilation state
- **Function Declarations**: All public APIs for code generation

### 2. **icg.c** - Core Implementation
Complete implementation featuring:
- **Symbol Table Management**
  - `symbol_table_init()` - Create and initialize symbol table
  - `symbol_table_insert()` - Add new symbols with scope tracking
  - `symbol_table_lookup()` - Find symbols in current scope
  - `symbol_table_exists()` - Check if symbol is declared
  
- **Quadruple Management**
  - `quad_list_init()` - Initialize quadruple list
  - `quad_list_add()` - Add quadruple with automatic resizing
  - `quad_list_print()` - Format and print quadruples
  - `icg_emit_quad()` - Generate and emit a quadruple

- **Temporary Variables & Labels**
  - `icg_get_temp_var()` - Generate unique temporary variables (t0, t1, ...)
  - `icg_get_label()` - Generate unique labels (L0, L1, ...)

- **AST Traversal**
  - `icg_generate()` - Main entry point for code generation
  - `icg_process_program()` - Process program node
  - `icg_process_statements()` - Process statement sequences
  - `icg_process_statement()` - Dispatch to appropriate statement handler
  - `icg_process_declaration()` - Handle variable declarations
  - `icg_process_assignment()` - Handle assignments
  - `icg_process_if_statement()` - Generate if-then-else code
  - `icg_process_while_statement()` - Generate while loop code
  - `icg_process_for_statement()` - Handle for loops
  - `icg_process_expression()` - Evaluate expressions
  - `icg_process_term()` - Handle term expressions
  - `icg_process_factor()` - Handle factor expressions

- **Operator Handling**
  - `icg_get_operator_type()` - Convert operator strings to operation types
  - `icg_operator_to_string()` - Convert operation types to strings

### 3. **icg_main.c** - Main Driver
Orchestrates the full compilation pipeline:
- Reads source program (program.txt or specified file)
- Initializes parser to generate AST
- Initializes ICG context
- Calls `icg_generate()` to produce quadruples
- Outputs results to both console and file
- Proper error handling and resource cleanup

### 4. **Makefile** - Build Configuration
Automated build system:
- Compiles ICG source files
- Rebuilds parser components as needed
- Handles dependencies correctly
- Provides `make clean`, `make`, `make run` targets

## Quadruple Operations Supported

| Category | Operations |
|----------|------------|
| **Arithmetic** | +, -, *, /, % |
| **Relational** | <, >, <=, >=, ==, != |
| **Assignment** | = |
| **Control Flow** | label, goto, if_goto |
| **Declarations** | declare |
| **Program** | end |

## Features

✅ **Three-Address Code**: Quadruple format (OP, ARG1, ARG2, RESULT)  
✅ **Symbol Table**: Tracks variable declarations with scope information  
✅ **Temporary Variables**: Automatic generation of t0, t1, t2, ...  
✅ **Label Generation**: Automatic generation of L0, L1, L2, ...  
✅ **Expression Evaluation**: Recursive descent with temporary variable allocation  
✅ **Control Flow**: If-then-else and while loop code generation  
✅ **Error Handling**: Graceful error handling and cleanup  
✅ **Formatted Output**: Professional table formatting for both console and file  
✅ **Scope Tracking**: Support for variable scope levels  

## File Structure

```
ICG-G24/
├── icg.h              ← Data structures and API
├── icg.c              ← Core implementation (500+ lines)
├── icg_main.c         ← Main driver (100+ lines)
├── makefile           ← Build configuration
└── README.md          ← Detailed documentation
```

## Building and Testing

### Build
```bash
cd ICG-G24
make clean && make
```

### Run
```bash
make run
# Or manually:
./icg ../program.txt icg_output.txt
```

### Sample Output
```
========== INTERMEDIATE CODE (QUADRUPLES) ==========
NUM   | OP              | ARG1            | ARG2            | RESULT         
------|-----------------|-----------------|-----------------|------------------
0     | declare         | int             | -               | x              
1     | declare         | int             | -               | y              
2     | =               | 5               | -               | x              
3     | end             | -               | -               | -              

Symbol Table:
Variable             | Type       | Scope     
--------------------|-----------|----------
x                    | int        | 0         
y                    | int        | 0
```

## Integration

The ICG integrates seamlessly with the existing Lexer and Parser:

```
program.txt
    ↓
[Lexer] → tokens
    ↓
[Parser] → AST
    ↓
[ICG] → Quadruples (THREE-ADDRESS CODE) ← NEW
    ↓
(Ready for optimization/code generation)
```

## Code Quality

- ✅ No compiler warnings (with -Wall -Wextra)
- ✅ Proper memory management (malloc/free)
- ✅ Error checking throughout
- ✅ Well-commented code
- ✅ Consistent naming conventions
- ✅ Modular design

## Dependencies

- Requires: Parser and Parse Tree from Parser-G24/
- Links with: parser.o, parse_tree.o
- No external dependencies

## Usage Examples

### Variable Declaration
```
Input: int x
Output: (declare, int, -, x)
```

### Assignment
```
Input: x = 5
Output: (=, 5, -, x)
```

### Arithmetic Expression
```
Input: y = x + 5 * 2
Output:
  (*, 5, 2, t0)
  (+, x, t0, t1)
  (=, t1, -, y)
```

### Conditional
```
Input: if x > 3 ...
Output:
  (>, x, 3, t0)
  (if_goto, t0, 0, L0)
  ... (body code)
  (goto, L1)
  (label, L0)
  (label, L1)
```

### Loop
```
Input: while i < 10 ...
Output:
  (label, L0)
  (<, i, 10, t0)
  (if_goto, t0, 0, L1)
  ... (body code)
  (goto, L0)
  (label, L1)
```

## Notes

- The ICG handles operator precedence through the recursive expression processing hierarchy
- Temporary variables are allocated on-demand during expression evaluation
- Symbol table prevents duplicate declarations in the same scope
- Labels enable proper jump targets for control flow statements
- Output is formatted in both console and file (icg_output.txt)

## Future Enhancements

Potential improvements for next phases:
- Optimize quadruples (remove redundant code, constant folding)
- Register allocation for temporary variables
- Code generation to assembly language
- Array support
- Function calls and parameters
- More complex expressions with proper precedence

---

**Status**: ✅ **COMPLETE** - ICG with quadruples fully implemented and tested
