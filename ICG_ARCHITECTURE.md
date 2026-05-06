# ICG Quadruples - Visual Architecture

## Compilation Pipeline

```
┌─────────────────────────────────────────────────────────────────┐
│                      COMPILER PIPELINE                         │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────┐
│   program.txt       │
│                     │
│  int x              │
│  int y              │
│  y = x + 5          │
│  if x > 3           │
│    y = y * 2        │
└────────────┬────────┘
             │
             ▼
┌─────────────────────┐
│     LEXER           │
│  (Lexer-G24)        │
│                     │
│  Tokenization       │
│  Error detection    │
└────────────┬────────┘
             │
             ▼
┌─────────────────────┐
│   Token Stream      │
│                     │
│  TOK_KEYWORD_INT    │
│  TOK_IDENTIFIER     │
│  TOK_ASSIGN         │
│  TOK_NUMBER         │
│  ...                │
└────────────┬────────┘
             │
             ▼
┌─────────────────────┐
│     PARSER          │
│  (Parser-G24)       │
│                     │
│  Syntax analysis    │
│  Parse tree build   │
└────────────┬────────┘
             │
             ▼
┌─────────────────────┐
│   Abstract Syntax   │
│   Tree (AST)        │
│                     │
│   ├─ Program        │
│   │  ├─ Statements  │
│   │  │  ├─ Declare  │
│   │  │  ├─ Assign   │
│   │  │  └─ If       │
│   │  └─ ...         │
└────────────┬────────┘
             │
             ▼
┌─────────────────────┐
│   ICG SYSTEM        │  ◄─── NEW!
│  (ICG-G24)          │
│                     │
│  AST Traversal      │
│  Symbol Table Build │
│  Quad Generation    │
└────────────┬────────┘
             │
             ▼
┌──────────────────────────┐
│   QUADRUPLES             │
│   (Three-Address Code)   │
│                          │
│  (declare, int, -, x)    │
│  (declare, int, -, y)    │
│  (+, x, 5, t0)           │
│  (=, t0, -, y)           │
│  (>, x, 3, t1)           │
│  (if_goto, t1, 0, L0)    │
│  (*, y, 2, t2)           │
│  (=, t2, -, y)           │
│  (label, L0)             │
│  (end)                   │
└──────────────────────────┘
             │
             ▼
         (Ready for)
    ┌─────────────────────┐
    │ • Optimization      │
    │ • Code Generation   │
    │ • Register Alloc    │
    └─────────────────────┘
```

## ICG System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                     ICG SUBSYSTEM                               │
└─────────────────────────────────────────────────────────────────┘

        ┌──────────────────────────────────────────────┐
        │    ICG CONTEXT (icg_main.c)                  │
        │  ┌────────────────────────────────────────┐  │
        │  │ Parser Integration                      │  │
        │  │ AST Loading                             │  │
        │  │ Context Initialization                  │  │
        │  │ Output Formatting                       │  │
        │  └────────────────────────────────────────┘  │
        └──────────────────┬───────────────────────────┘
                           │
                           ▼
        ┌──────────────────────────────────────────────┐
        │    GENERATION ENGINE (icg.c)                │
        │                                              │
        │  ┌─────────────────────────────────────┐   │
        │  │ AST Traversal Module                │   │
        │  │ • process_program()                 │   │
        │  │ • process_statements()              │   │
        │  │ • process_statement()               │   │
        │  │ • process_declaration()             │   │
        │  │ • process_assignment()              │   │
        │  │ • process_if_statement()            │   │
        │  │ • process_while_statement()         │   │
        │  │ • process_for_statement()           │   │
        │  │ • process_expression()              │   │
        │  │ • process_term()                    │   │
        │  │ • process_factor()                  │   │
        │  └─────────────────────────────────────┘   │
        │                                              │
        │  ┌─────────────────────────────────────┐   │
        │  │ Quadruple Emission                  │   │
        │  │ • emit_quad()                       │   │
        │  │ • add() to quad_list                │   │
        │  └─────────────────────────────────────┘   │
        │                                              │
        │  ┌─────────────────────────────────────┐   │
        │  │ Symbol Management                   │   │
        │  │ • insert()                          │   │
        │  │ • lookup()                          │   │
        │  │ • exists()                          │   │
        │  └─────────────────────────────────────┘   │
        │                                              │
        │  ┌─────────────────────────────────────┐   │
        │  │ Utility Functions                   │   │
        │  │ • get_temp_var() → t0, t1, ...     │   │
        │  │ • get_label() → L0, L1, ...        │   │
        │  └─────────────────────────────────────┘   │
        │                                              │
        └──────────────────┬───────────────────────────┘
                           │
            ┌──────────────┼──────────────┐
            ▼              ▼              ▼
    ┌────────────┐ ┌──────────────┐ ┌─────────────┐
    │ Symbol     │ │ Quadruple    │ │ Operator    │
    │ Table      │ │ List         │ │ Mapping     │
    ├────────────┤ ├──────────────┤ ├─────────────┤
    │ • name     │ │ • count      │ │ • +  →OP_ADD│
    │ • type     │ │ • capacity   │ │ • -  →OP_SUB│
    │ • scope    │ │ • quads[]    │ │ • *  →OP_MUL│
    │ • init     │ │   ├─ op      │ │ • <  →OP_LT │
    │            │ │   ├─ arg1    │ │ • etc...    │
    │            │ │   ├─ arg2    │ │             │
    │            │ │   └─ result  │ │             │
    └────────────┘ └──────────────┘ └─────────────┘
```

## Expression Evaluation Flow

```
Input: y = x + 5 * 2

           ┌─────────────────────────────┐
           │   parse_assignment()        │
           │   y = <expression>          │
           └──────────────┬──────────────┘
                          │
                          ▼
           ┌─────────────────────────────┐
           │  parse_expression()         │
           │  x + 5 * 2                  │
           └──────────────┬──────────────┘
                          │
           ┌──────────────┼──────────────┐
           │              │              │
      ┌────▼─────┐  ┌─────▼────┐  ┌──────▼────┐
      │ factor   │  │ operator │  │ term      │
      │ x        │  │ +        │  │ 5 * 2     │
      └──────────┘  └──────────┘  └──────┬────┘
                                         │
                          ┌──────────────┼──────────────┐
                          │              │              │
                     ┌────▼────┐  ┌─────▼────┐  ┌─────▼────┐
                     │ factor  │  │ operator │  │ factor   │
                     │ 5       │  │ *        │  │ 2        │
                     └─────────┘  └──────────┘  └─────────┘

Generated Quadruples:

Step 1: Process 5 * 2 (higher precedence)
        (*, 5, 2, t0)

Step 2: Process x + t0
        (+, x, t0, t1)

Step 3: Assign to y
        (=, t1, -, y)

Result:
        (*, 5, 2, t0)
        (+, x, t0, t1)
        (=, t1, -, y)
```

## Control Flow - If Statement

```
Input:
  if x > 3
    y = 1

Code Generation:

    ┌────────────────────────────────────┐
    │ (>, x, 3, t0)                      │
    │ Evaluate condition                 │
    └─────────────┬──────────────────────┘
                  │
    ┌─────────────▼──────────────────────┐
    │ (if_goto, t0, 0, L0)               │
    │ Jump to L0 if condition FALSE      │
    └─────────────┬──────────────────────┘
                  │
    ┌─────────────▼──────────────────────┐
    │ (=, 1, -, y)                       │
    │ Body (executes if condition TRUE)  │
    └─────────────┬──────────────────────┘
                  │
    ┌─────────────▼──────────────────────┐
    │ (goto, L1)                         │
    │ Skip else block (if exists)        │
    └─────────────┬──────────────────────┘
                  │
    ┌─────────────▼──────────────────────┐
    │ (label, L0)                        │
    │ Else label (if no else, just skip) │
    └─────────────┬──────────────────────┘
                  │
    ┌─────────────▼──────────────────────┐
    │ (label, L1)                        │
    │ End of if statement                │
    └────────────────────────────────────┘

Visual Flow:

    condition true?  ┌─────────────┐
    ───────────────►│ Execute body│
    │               └──────┬──────┘
    │                      │
    │                      ▼
    │             (goto L1 - exit)
    │                      │
    │                      ▼
    └─────────────────────(L1)
```

## Control Flow - While Loop

```
Input:
  while i < 10
    x = x + 1

Code Generation:

    ┌────────────────────────────────────┐
    │ (label, L0)                        │
    │ Loop entry point                   │
    └─────────────┬──────────────────────┘
                  │
    ┌─────────────▼──────────────────────┐
    │ (<, i, 10, t0)                     │
    │ Evaluate condition                 │
    └─────────────┬──────────────────────┘
                  │
    ┌─────────────▼──────────────────────┐
    │ (if_goto, t0, 0, L1)               │
    │ Jump to L1 if condition FALSE      │
    └─────────────┬──────────────────────┘
                  │
    ┌─────────────▼──────────────────────┐
    │ (+, x, 1, t1)                      │
    │ (=, t1, -, x)                      │
    │ Loop body (executes while TRUE)    │
    └─────────────┬──────────────────────┘
                  │
    ┌─────────────▼──────────────────────┐
    │ (goto, L0)                         │
    │ Jump back to condition             │
    └─────────────┬──────────────────────┘
                  │
    ┌─────────────▼──────────────────────┐
    │ (label, L1)                        │
    │ Exit label                         │
    └────────────────────────────────────┘

Visual Flow:

    (L0)◄────────────────────┐
      │                      │
      ▼ condition true?      │
    (body)                   │
      │                      │
      ▼ (goto L0)───────────┘
      
    condition false?
      │
      ▼
    (L1) exit
```

---

**This architecture enables:**
- Clean separation of concerns
- Efficient AST traversal
- Automatic temporary/label generation
- Proper operator precedence
- Control flow patterns
- Symbol tracking
- Professional output formatting

**Status**: ✅ Complete and production-ready
