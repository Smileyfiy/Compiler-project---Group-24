# Parser Implementation - Project Workplan

**Group 24 – Compiler Construction**

**Members:**
- ODHIAMBO JOHN OTIENO - SCS3/147357/2023
- MICHAEL MUTETI - SCS3/147532/2024
- ABBA BIKO WERE - SCS3/146750/2023
- MARK WAFULA - SCS3/146670/2023

**Project Duration:** 3 weeks (April 15 - April 24, 2026)
**Live Demo Date:** April 24, 2026

---

## 1. Implementation Approach

**Decision: Recursive-Descent Predictive LL(1) Parser**

**Rationale:**
- LL(1) properties ensure deterministic parsing with single lookahead
- Recursive descent is intuitive and easier to debug
- Suitable for the mini-grammar complexity
- Direct integration with lexer tokens
- Clear error reporting at grammar rule level

---

## 2. Grammar Specification

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

---

## 3. Tasks/Modules/Functions Breakdown

| # | Task/Module | Description | Estimated Effort | Responsibility | Status |
|---|---|---|---|---|---|
| 1 | Token Type Definition | Define all token types (keywords, operators, identifiers, numbers) | 0.5 days | JOHN OTIENO | ✅ Complete |
| 2 | Lexer Integration | Create token reader interface for parser | 1 day | JOHN OTIENO | ✅ Complete |
| 3 | Parse Tree Data Structure | Design AST node structure for parse tree representation | 1.5 days | MICHAEL MUTETI | ✅ Complete |
| 4 | Parser Core (Base Functions) | Implement parser initialization, token management, error handling | 1 day | MICHAEL MUTETI | ✅ Complete |
| 5 | Statement Parsing Functions | Implement Declaration, Assignment, IfStatement, WhileStatement, ForStatement | 2 days | ABBA BIKO WERE | ⏳ In Progress |
| 6 | Expression Parsing Functions | Implement Expression, AddExpr, Term, MulExpr, Factor | 1.5 days | ABBA BIKO WERE | ⏳ In Progress |
| 7 | Parse Tree Generation | Modify parser to build and return AST nodes | 1.5 days | MARK WAFULA | ⏳ In Progress |
| 8 | Parse Tree Printing | Implement visual representation of parse tree | 1 day | MARK WAFULA | ⏳ In Progress |
| 9 | Error Handling & Recovery | Implement detailed error messages and recovery | 1 day | JOHN OTIENO | ⏳ In Progress |
| 10 | Sample Test Programs | Create comprehensive test cases | 1 day | MICHAEL MUTETI | ⏳ In Progress |
| 11 | Integration Testing | Test lexer → parser pipeline | 1 day | ABBA BIKO WERE | ⏳ In Progress |
| 12 | Documentation & Report | Create final report with screenshots and token traces | 1 day | MARK WAFULA | ⏳ In Progress |

**Total Estimated Effort:** ~14.5 days (distributed over 3 weeks)

---

## 4. Detailed Task Assignments & Timeline

### **Week 1 (April 15-19): Foundation & Core Implementation**

#### **April 15 (Day 1)**
- **Token Definition & Parser Setup** (JOHN OTIENO - 0.5 days)
  - Define all token types
  - Create lexer integration interface
  - Ensure token reading works correctly
  
- **Parse Tree Data Structure** (MICHAEL MUTETI - 1.5 days)
  - Design AST node structure
  - Create node creation and linking functions
  - Test basic node operations

#### **April 16-17 (Days 2-3)**
- **Parser Core Functions** (MICHAEL MUTETI - 1 day)
  - Implement parser initialization
  - Token management functions
  - Basic error handling framework
  
- **Statement Parsing** (ABBA BIKO WERE - 1 day)
  - Implement Declaration parsing (int, char)
  - Implement Assignment parsing
  - Test with simple statements

#### **April 18 (Day 4)**
- **Control Structure Parsing** (ABBA BIKO WERE - 1 day)
  - Implement IfStatement parsing
  - Implement WhileStatement parsing
  - Implement ForStatement parsing

#### **April 19 (Day 5)**
- **Expression Parsing** (ABBA BIKO WERE - 0.5 days)
  - Implement Expression, AddExpr, Term, MulExpr, Factor
  - Test operator precedence

---

### **Week 2 (April 20-22): Parse Tree & Output**

#### **April 20-21 (Days 6-7)**
- **Parse Tree Generation** (MARK WAFULA - 1.5 days)
  - Modify parsing functions to return AST nodes
  - Implement node linking logic
  - Test tree generation

#### **April 21 (Day 8)**
- **Parse Tree Visualization** (MARK WAFULA - 1 day)
  - Implement tree printing functions
  - Add indentation and formatting
  - Create visual output format

#### **April 22 (Day 9)**
- **Error Handling** (JOHN OTIENO - 1 day)
  - Implement detailed error messages
  - Add error recovery strategies
  - Test with malformed input

---

### **Week 3 (April 23-24): Testing & Delivery**

#### **April 23 (Day 10)**
- **Sample Programs & Testing** (MICHAEL MUTETI - 1 day)
  - Create comprehensive test cases
  - Test lexer → parser pipeline
  - Generate token traces
  
#### **April 23 (Day 11)**
- **Integration Testing** (ABBA BIKO WERE - 1 day)
  - Full end-to-end testing
  - Verify all grammar rules work
  - Test error cases

#### **April 24 (Day 12-14)**
- **Documentation & Report** (MARK WAFULA - 1 day)
  - Create final report
  - Prepare screenshots
  - Document token traces
  
- **Live Demo Preparation** (ALL MEMBERS - 2 days)
  - Practice demo presentation
  - Prepare explanations
  - Test all components

---

## 5. Deliverables

### **Source Code**
- ✅ [parser.h](parser.h) - Header with token definitions and data structures
- ✅ [parser.c](parser.c) - Parser implementation with all grammar rules
- ✅ [main.c](main.c) - Driver program
- 📝 parse_tree.h/c - Parse tree generation functions (in progress)
- 📝 parse_tree_print.h/c - Tree visualization (in progress)

### **Documentation**
- ✅ [Grammar Specification](readme.md) with FIRST/FOLLOW sets
- ✅ Workplan (this document)
- 📝 Token trace from sample program
- 📝 Parse tree outputs
- 📝 Final assessment report

### **Test Cases**
- ✅ Simple declaration test
- ✅ Expression with operators
- ✅ If statement
- ✅ While loop
- ✅ For loop
- 📝 Complex nested example
- 📝 Error case examples

---

## 6. Technical Implementation Details

### **Parser State Machine**
```
Token Stream → Lexer Interface → Parser Functions → AST Nodes → Tree Printer
```

### **Function Call Hierarchy**
```
parse_program()
├── parse_statements()
│   ├── parse_statement()
│   │   ├── parse_declaration()
│   │   ├── parse_assignment()
│   │   ├── parse_if_statement()
│   │   ├── parse_while_statement()
│   │   ├── parse_for_statement()
│   │   └── parse_expression()
│   │       ├── parse_term()
│   │       │   ├── parse_factor()
│   │       │   └── parse_mul_expr()
│   │       └── parse_add_expr()
│   └── [recursion for multiple statements]
└── EOF check
```

### **Error Handling Strategy**
- Token mismatch detection
- Missing keyword/operator errors
- Unexpected token messages
- Error recovery via skip-to-next-statement
- Line number tracking (optional enhancement)

---

## 7. Testing Strategy

### **Unit Testing**
- Test each parsing function individually
- Verify token consumption
- Check error detection

### **Integration Testing**
1. Lexer output → Parser input
2. Simple statements → Complex expressions
3. Nested control structures
4. Error cases and recovery

### **Test Case Categories**
| Category | Examples | Status |
|---|---|---|
| Declarations | `int x`, `char y` | ✅ Complete |
| Assignments | `x = 5`, `x = y + 2` | ✅ Complete |
| Expressions | `5 + 3`, `x * 2 % 3` | ✅ Complete |
| Control Flow | `if x ...`, `while x ...`, `for x=1 ...` | ✅ Complete |
| Compound | Multiple statements, nested if/while | ⏳ In Progress |
| Error Cases | Missing tokens, type mismatches | ⏳ In Progress |

---

## 8. Success Criteria

- [x] Parser accepts all valid mini-grammar programs
- [x] Parser rejects invalid programs with error messages
- [ ] Parse tree is generated for successful parses
- [ ] Parse tree is displayed in readable format
- [ ] Lexer output feeds directly into parser
- [ ] All team members can explain their assigned components
- [ ] Demo successful on April 24

---

## 9. Risk Mitigation

| Risk | Probability | Mitigation |
|---|---|---|
| Scope creep | Medium | Stick to grammar specification, defer enhancements |
| Integration issues | Low | Test lexer-parser connection early (Day 2) |
| Time constraints | Medium | Work in parallel on non-dependent tasks |
| Member unavailability | Low | Document tasks clearly for handoff |

---

## 10. Notes for Live Demo

**Demo Flow:**
1. Show source program (mini-language code)
2. Run lexer, show token output
3. Feed tokens to parser
4. Display parse tree output
5. Each member explains their contribution

**Materials to Prepare:**
- Sample program files
- Expected token output
- Expected parse trees
- Error example with error messages

---

**Document Prepared:** April 15, 2026
**Next Review:** April 20, 2026
