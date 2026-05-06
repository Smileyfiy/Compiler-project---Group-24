# ICG Quick Reference

## Quick Start

```bash
# Navigate to ICG directory
cd ICG-G24

# Build
make

# Run on test program
./icg ../program.txt icg_output.txt

# Or use make
make run
```

## Quadruple Format

**Format**: `(Operation, Argument1, Argument2, Result)`

### Examples

| Code | Quadruple |
|------|-----------|
| `int x` | `(declare, int, -, x)` |
| `x = 5` | `(=, 5, -, x)` |
| `y = x + 5` | `(+, x, 5, t0)` then `(=, t0, -, y)` |
| `z = x * 2 + y` | `(*, x, 2, t0)` then `(+, t0, y, t1)` then `(=, t1, -, z)` |

## Operations Reference

```c
OP_ASSIGN       (=)
OP_ADD          (+)
OP_SUB          (-)
OP_MUL          (*)
OP_MOD          (%)
OP_DIV          (/)
OP_LT           (<)
OP_GT           (>)
OP_LE           (<=)
OP_GE           (>=)
OP_EQ           (==)
OP_NE           (!=)
OP_LABEL        (label)
OP_GOTO         (goto)
OP_IF_GOTO      (if_goto)
OP_DECLARE      (declare)
OP_END          (end)
```

## Key APIs

### Initialization
```c
ICGContext* icg_init();                 // Create context
void icg_free(ICGContext *ctx);         // Free context
```

### Code Generation
```c
void icg_generate(ParseTreeNode *ast, ICGContext *ctx);
void icg_emit_quad(ICGContext *ctx, OperationType op, 
                   const char *arg1, const char *arg2, const char *result);
```

### Symbol Table
```c
void symbol_table_insert(SymbolTable *st, const char *name, 
                         const char *type, int scope);
Symbol* symbol_table_lookup(SymbolTable *st, const char *name);
int symbol_table_exists(SymbolTable *st, const char *name);
```

### Temporary Variables & Labels
```c
char* icg_get_temp_var(ICGContext *ctx);  // Generate t0, t1, ...
char* icg_get_label(ICGContext *ctx);     // Generate L0, L1, ...
```

### Output
```c
void quad_list_print(QuadrupleList *ql, FILE *f);
```

## If Statement Pattern

```c
// Condition evaluation
icg_emit_quad(ctx, OP_GT, "x", "3", "t0");        // (>, x, 3, t0)

// Conditional jump to else
icg_emit_quad(ctx, OP_IF_GOTO, "t0", "0", "L0");  // (if_goto, t0, 0, L0)

// Body code (only executes if condition true)
icg_emit_quad(ctx, OP_ASSIGN, "1", "", "y");      // (=, 1, -, y)

// Unconditional jump to end
icg_emit_quad(ctx, OP_GOTO, "L1", "", "");        // (goto, L1)

// Else label
icg_emit_quad(ctx, OP_LABEL, "L0", "", "");       // (label, L0)

// End label
icg_emit_quad(ctx, OP_LABEL, "L1", "", "");       // (label, L1)
```

## While Loop Pattern

```c
// Loop label
icg_emit_quad(ctx, OP_LABEL, "L0", "", "");       // (label, L0)

// Condition
icg_emit_quad(ctx, OP_LT, "i", "10", "t0");       // (<, i, 10, t0)

// Jump to exit if condition false
icg_emit_quad(ctx, OP_IF_GOTO, "t0", "0", "L1");  // (if_goto, t0, 0, L1)

// Body code
icg_emit_quad(ctx, OP_ASSIGN, "1", "", "x");      // (=, 1, -, x)

// Jump back to condition
icg_emit_quad(ctx, OP_GOTO, "L0", "", "");        // (goto, L0)

// Exit label
icg_emit_quad(ctx, OP_LABEL, "L1", "", "");       // (label, L1)
```

## Expression Evaluation

### Evaluating: `y = a + b * c`

**Step 1**: Process multiplication first (higher precedence)
```
(*, b, c, t0)  // Temporary stores b*c
```

**Step 2**: Process addition
```
(+, a, t0, t1) // Temporary stores a + (b*c)
```

**Step 3**: Assign result
```
(=, t1, -, y)  // Store in y
```

**Final Quadruples**:
1. `(*, b, c, t0)`
2. `(+, a, t0, t1)`
3. `(=, t1, -, y)`

## Files

- `icg.h` - Structures and prototypes (~200 lines)
- `icg.c` - Implementation (~540 lines)
- `icg_main.c` - Driver (~100 lines)
- `makefile` - Build rules
- `README.md` - Full documentation
- `QUICK_REFERENCE.md` - This file

## Output Files

- **Console**: Pretty-printed quadruple table with ASCII formatting
- **icg_output.txt**: Same content formatted for file storage

## Compilation Info

```
gcc -Wall -Wextra -std=c11 -I.. -c icg.c
gcc -Wall -Wextra -std=c11 -I.. -o icg icg_main.o icg.o ../Parser-G24/parser.o ../Parser-G24/parse_tree.o
```

## Troubleshooting

| Issue | Solution |
|-------|----------|
| `error: undefined reference to parser_init` | Make sure parser objects are compiled: `make clean && make` |
| `error: multiple definition of main` | Don't link with Parser-G24/main.o, only parser.o and parse_tree.o |
| Empty quadruple list | Check that AST is being generated properly by the parser |
| Segmentation fault | Check for NULL pointers in AST or input file issues |

---

**Last Updated**: May 2026  
**Status**: Complete and Tested
