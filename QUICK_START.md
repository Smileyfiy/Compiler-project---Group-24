# How to Use the Unified Compiler Pipeline

## Simple 3-Step Process

### Step 1: Edit Your Program
```bash
# Option A: Edit with text editor
nano program.txt

# Option B: Use echo/printf
printf "int x\nx = 10\n" > program.txt
```

### Step 2: Run the Pipeline
```bash
./compile.sh
```

### Step 3: View Outputs

The script automatically:
- ✅ Shows your source program
- ✅ Runs the **Lexer** and displays **tokens**
- ✅ Runs the **Parser** and displays **parse tree**

---

## What You'll See

### Complete Output Flow

```
═══════════════════════════════════════════════════════════════
     COMPILER PIPELINE: Lexer → Parser
═══════════════════════════════════════════════════════════════

STEP 1: SOURCE PROGRAM
────────────────────────────────────────────────────────────
int x
x = 10
if x
x = x + 5

STEP 2: LEXICAL ANALYSIS (Lexer-G24)
────────────────────────────────────────────────────────────
Running Lexer:
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

STEP 3: SYNTAX ANALYSIS (Parser-G24)
────────────────────────────────────────────────────────────
Running Parser:
=== LL(1) Parser Starting ===

=== Parsing Successful ===

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
                ├─ Number : "10"
              ├─ Term
                ├─ ε
            ├─ Term
              ├─ ε
...

═══════════════════════════════════════════════════════════════
✓ Compilation pipeline completed successfully
═══════════════════════════════════════════════════════════════
```

---

## Understanding Lexer Output

Each line shows: `TOKEN_TYPE: token_value`

| Token Type | Example | What It Is |
|---|---|---|
| KEYWORD | `KEYWORD: int` | Reserved words |
| IDENTIFIER | `IDENTIFIER: x` | Variable names |
| NUMBER | `NUMBER: 10` | Numeric literals |
| OPERATOR | `OPERATOR: =` | Operators (+, *, %, =) |

---

## Understanding Parser Output

The parse tree shows the **structure** of your program according to the grammar:

```
├─ Program                    ← Root node
  ├─ Statements              ← Sequence of statements
    ├─ Statement             ← Single statement
      ├─ Declaration         ← Variable declaration
        ├─ Keyword : "int"   ← Type keyword
        ├─ Identifier : "x"  ← Variable name
```

### Tree Indentation = Grammar Nesting

**More indentation** = deeper in grammar structure

```
Program (depth 0)
  Statements (depth 1)
    Statement (depth 2)
      Declaration (depth 3)
        Keyword (depth 4)
```

---

## Example Programs to Try

### Example 1: Simple Declaration
```bash
printf "int x\n" > program.txt
./compile.sh
```

**Expected Lexer Output**: `KEYWORD: int` + `IDENTIFIER: x`  
**Expected Parser Output**: Declaration node with keyword and identifier

---

### Example 2: Assignment with Expression
```bash
printf "int x\nx = 5 + 3\n" > program.txt
./compile.sh
```

**Expected Lexer Output**: Tokens for declaration + assignment + expression  
**Expected Parser Output**: Declaration + Assignment + Expression with operator precedence

---

### Example 3: All Features
```bash
printf "int count\nint sum\ncount = 5\nwhile count\nsum = sum + 1\n" > program.txt
./compile.sh
```

**Expected Lexer Output**: Multiple keywords, identifiers, operators, numbers  
**Expected Parser Output**: Multiple statements including while loop node

---

## Common Patterns

### Declaration
```
int variableName
```

### Assignment
```
variableName = expression
```

### If Statement
```
if expression
statement
```

### While Loop
```
while expression
statement
```

### For Loop
```
for variableName = expression
statement
```

### Expressions
- Simple: `5` or `x`
- Addition: `x + 5`
- Multiplication/Modulo: `x * 2` or `x % 3`
- Combined: `x + 5 * 2`

---

## Troubleshooting

### "program.txt not found"
**Solution**: Make sure you're in the Compiler-project---Group-24 directory:
```bash
cd /home/john/dev/projects/Compiler-project---Group-24
./compile.sh
```

### Lexer shows unexpected tokens
**Check**: 
- Variable names must start with a letter: `x` (✓) not `1x` (✗)
- Use spaces to separate tokens: `int x` not `intx`

### Parser shows error messages
**Example Error**:
```
PARSE ERROR: Expected identifier
ERROR: Expected EOF
```

**Common Causes**:
- Missing declaration: `x = 5` without `int x`
- Syntax error in expression
- Extra characters that don't match grammar

---

## Pipeline Architecture

```
┌─────────────────────────────────────────────────────┐
│              program.txt (input)                    │
│         (Your mini-language source code)            │
└─────────────┬───────────────────────────────────────┘
              │
              ↓
      ┌───────────────┐
      │ Lexer-G24     │  Converts characters → Tokens
      │ (lexer)       │
      └───────┬───────┘
              │
              │ Tokens: KEYWORD, IDENTIFIER, NUMBER, OPERATOR
              │
              ↓
      ┌───────────────┐
      │ Parser-G24    │  Converts tokens → Parse Tree
      │ (parser)      │
      └───────┬───────┘
              │
              │ Parse Tree showing grammar structure
              │
              ↓
    ┌─────────────────────┐
    │ Output to Console   │  Both lexer & parser outputs
    │ (visible on screen) │
    └─────────────────────┘
```

---

## Key Points for Demonstration

✅ **Single Input**: One `program.txt` file  
✅ **Two-Stage Process**: Lexer → Parser  
✅ **Separate Outputs**: Token list + Parse tree shown distinctly  
✅ **Fully Automated**: One command (`./compile.sh`) does everything  
✅ **Color-Coded**: Visual separation of stages  
✅ **Error Handling**: Reports issues clearly  

---

## For Your Live Demo (April 24)

1. **Prepare**: Have a good program.txt ready
2. **Demonstrate**: Run `./compile.sh`
3. **Explain**: 
   - Point out tokens from lexer
   - Show parse tree structure from parser
   - Highlight which grammar rules were used
4. **Ask**: Let assessors provide custom input

---

## Quick Reference Card

```bash
# Edit your program
nano program.txt

# OR create with printf
printf "int x\nx = 5\n" > program.txt

# Run the complete pipeline
./compile.sh

# That's it! You'll see:
# 1. Your source program
# 2. Lexer tokens
# 3. Parser tree
```

**Remember**: One file (`program.txt`), one command (`./compile.sh`), two outputs (lexer + parser) ✅
