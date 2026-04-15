# ✅ UNIFIED COMPILER PIPELINE - READY TO USE

## What You Now Have

A **complete, working compiler** that:
1. Takes ONE `program.txt` input file
2. Runs it through the **LEXER** (Lexer-G24) → Shows tokens
3. Runs it through the **PARSER** (Parser-G24) → Shows parse tree
4. Displays **BOTH outputs separately** in one organized execution

---

## 🚀 How to Use (3 Steps)

### Step 1: Create Your Program
```bash
cd /home/john/dev/projects/Compiler-project---Group-24

# Edit program.txt with your code
nano program.txt

# OR use command line:
printf "int x\nx = 5 + 3\n" > program.txt
```

### Step 2: Run the Pipeline
```bash
./compile.sh
```

### Step 3: View Results
The script automatically outputs:
- ✅ **STEP 1**: Your source program
- ✅ **STEP 2**: Lexer tokens (word by word breakdown)
- ✅ **STEP 3**: Parser tree (grammar structure)

---

## 📊 Real Example

### Your program.txt:
```
int x
int y
x = 5
y = 10
if x
y = y * 2
```

### Running:
```bash
./compile.sh
```

### You'll see:

#### STEP 1: SOURCE PROGRAM
```
int x
int y
x = 5
y = 10
if x
y = y * 2
```

#### STEP 2: LEXICAL ANALYSIS (Lexer)
```
KEYWORD: int
IDENTIFIER: x
KEYWORD: int
IDENTIFIER: y
IDENTIFIER: x
OPERATOR: =
NUMBER: 5
IDENTIFIER: y
OPERATOR: =
NUMBER: 10
KEYWORD: if
IDENTIFIER: x
IDENTIFIER: y
OPERATOR: =
IDENTIFIER: y
OPERATOR: *
NUMBER: 2
```

#### STEP 3: SYNTAX ANALYSIS (Parser)
```
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
        ├─ Declaration
          ├─ Keyword : "int"
          ├─ Identifier : "y"
      ├─ Statements
        ├─ Statement
          ├─ Assignment
            ├─ Identifier : "x"
            ├─ Operator : "="
            ├─ Expression
              ├─ Term
                ├─ Factor
                  ├─ Number : "5"
                ├─ Term
                  ├─ ε
              ├─ Term
                ├─ ε
        ├─ Statements
          ├─ Statement
            ├─ Assignment
              ├─ Identifier : "y"
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
                  ├─ Term
                    ├─ Factor
                      ├─ Identifier : "x"
                    ├─ Term
                      ├─ ε
                  ├─ Term
                    ├─ ε
                ├─ Statement
                  ├─ Assignment
                    ├─ Identifier : "y"
                    ├─ Operator : "="
                    ├─ Expression
                      ├─ Term
                        ├─ Factor
                          ├─ Identifier : "y"
                        ├─ Term
                          ├─ Operator : "*"
                          ├─ Factor
                            ├─ Number : "2"
                          ├─ Term
                            ├─ ε
                      ├─ Term
                        ├─ ε
            ├─ Statements
              ├─ ε

═══════════════════════════════════════════════════════════════
✓ Compilation pipeline completed successfully
═══════════════════════════════════════════════════════════════
```

---

## 📚 Files You Have

```
/home/john/dev/projects/Compiler-project---Group-24/
│
├── program.txt              ← EDIT THIS (your input code)
├── compile.sh               ← RUN THIS (the pipeline)
│
├── README.md                ← Overview of project
├── QUICK_START.md           ← Quick examples
├── UNIFIED_PIPELINE.md      ← How it works
│
├── Lexer-G24/               ← Stage 1: Tokenization
│   ├── lexer               (binary)
│   ├── lexer.c
│   └── readme.md
│
└── Parser-G24/             ← Stage 2: Parse Tree
    ├── parser              (binary)
    ├── parser.c
    ├── parse_tree.c
    └── readme.md
```

---

## 🎯 Key Features

✅ **Single Input File**: One `program.txt` for everything  
✅ **Two Components**: Lexer → Parser  
✅ **Separate Outputs**: Tokens and parse tree shown distinctly  
✅ **Fully Automated**: One command (`./compile.sh`) runs both  
✅ **Auto-Compilation**: Compiles if binaries missing  
✅ **Color Output**: Visual separation of stages  
✅ **Error Handling**: Clear error messages if parse fails  

---

## 🔤 What the Lexer Shows

Each token on a line by itself: `TOKEN_TYPE: value`

```
KEYWORD: int        ← Language keyword
IDENTIFIER: count   ← Variable name
OPERATOR: =         ← Operator symbol
NUMBER: 10          ← Numeric value
```

---

## 🌳 What the Parser Shows

The grammar rules applied, formatted as a tree:

```
├─ Program                   ← Root (start symbol)
  ├─ Statements             ← Sequence of statements
    ├─ Statement            ← Single statement
      ├─ Declaration        ← Variable declaration
        ├─ Keyword : "int"  ← Type
        ├─ Identifier : "x" ← Name
      ├─ Assignment         ← Variable assignment
        ├─ Identifier : "y" ← Variable
        ├─ Operator : "="   ← Assignment operator
        ├─ Expression       ← Right-hand side
          ├─ Term           ← Multiplication/modulo
            ├─ Factor       ← Atomic value
              ├─ Number : "5"
```

**Tree depth** = Grammar nesting level  
**More indentation** = Deeper in structure

---

## 💡 Quick Examples to Try

### Example 1: Simplest
```bash
echo "int x" > program.txt && ./compile.sh
```

### Example 2: With Assignment
```bash
printf "int x\nx = 10\n" > program.txt && ./compile.sh
```

### Example 3: With Expression
```bash
printf "int x\nx = 5 + 3 * 2\n" > program.txt && ./compile.sh
```

### Example 4: With If Statement
```bash
printf "int x\nif x\nx = 10\n" > program.txt && ./compile.sh
```

### Example 5: With While Loop
```bash
printf "int n\nwhile n\nn = n + 1\n" > program.txt && ./compile.sh
```

### Example 6: Complex
```bash
printf "int a\nint b\na = 5\nb = 10\nif a\na = a + b\n" > program.txt && ./compile.sh
```

---

## 🎓 For Your Demo on April 24

1. **Show the script**: `cat compile.sh` (if needed)
2. **Show your input**: `cat program.txt`
3. **Run it**: `./compile.sh`
4. **Explain outputs**:
   - First part: Lexer breaks code into words (tokens)
   - Second part: Parser shows grammar structure (parse tree)
5. **Let assessors try**: "You can edit program.txt with any valid code"

---

## ✨ Pipeline Architecture (Visual)

```
                    program.txt
                        │
                        │ (input)
                        ↓
         ┌──────────────────────────────┐
         │   compile.sh (orchestrator)  │
         └──────────────────────────────┘
                        │
            ┌───────────┴───────────┐
            │                       │
            ↓                       ↓
      ┌──────────────┐      ┌──────────────┐
      │  Lexer-G24   │      │  Parser-G24  │
      │   (lexer)    │      │   (parser)   │
      └──────┬───────┘      └──────┬───────┘
             │                     │
             │ Tokens              │ Parse Tree
             ↓                     ↓
      ┌──────────────┐      ┌──────────────┐
      │Keyword       │      │├─ Program    │
      │Identifier    │      │  ├─ Statement│
      │Number        │      │  ├─ Expr    │
      │Operator      │      │  └─ ...      │
      └──────────────┘      └──────────────┘
             │                     │
             └─────────┬───────────┘
                       │
                       ↓
            ┌──────────────────────┐
            │  CONSOLE OUTPUT      │
            │                      │
            │ Step 1: Source       │
            │ Step 2: Lexer out    │
            │ Step 3: Parser out   │
            └──────────────────────┘
```

---

## 🔧 Troubleshooting

### "Command not found: compile.sh"
```bash
# Make sure you're in the right directory
cd /home/john/dev/projects/Compiler-project---Group-24
ls -la compile.sh

# Make it executable (should already be)
chmod +x compile.sh

# Run it
./compile.sh
```

### "program.txt not found"
```bash
# Create it
echo "int x" > program.txt

# Or edit it
nano program.txt

# Then run
./compile.sh
```

### Parse error
Check your syntax:
- Variables must be declared: `int x` before using `x`
- Keywords must be spelled right: `if`, `while`, `for`, `int`, `char`
- Expressions use: `+`, `*`, `%` (no `-` or `/`)
- Simple example: `int x\nx = 5\n`

---

## ✅ Everything Working?

Run this quick test:
```bash
cd /home/john/dev/projects/Compiler-project---Group-24
echo "int x\nx = 5" > program.txt
./compile.sh
```

You should see:
- ✓ Source program
- ✓ Lexer tokens
- ✓ Parser tree
- ✓ Success message

---

## 📖 For More Details

| Need | Read |
|---|---|
| Quick examples | QUICK_START.md |
| How it works | UNIFIED_PIPELINE.md |
| Who did what | WORKPLAN.md |
| Project status | PROJECT_STATUS.md |
| Lexer details | Lexer-G24/readme.md |
| Parser details | Parser-G24/readme.md |

---

## 🎉 Summary

**You now have a complete compiler that:**
- Takes ONE input file (`program.txt`)
- Produces TWO outputs (lexer tokens + parser tree)
- Works with ONE command (`./compile.sh`)
- Shows everything SEPARATE and CLEAR
- Handles ANY valid mini-language program

**Ready for your April 24, 2026 demo!** ✅

---

**Current Status**: READY TO USE
**Test the pipeline now with**: `./compile.sh`
