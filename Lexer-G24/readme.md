# Mini-Grammar Lexical Analyzer

This project implements a **lexical analyzer (scanner)** for a mini programming language grammar based on c programming language.

The scanner reads a source program and converts it into tokens using a **Deterministic Finite Automaton (DFA)**.

## Key Feature: Keyword Prefix Recognition

The lexer intelligently recognizes keywords even when they appear concatenated with other characters. For example, `ifwhile` is tokenized as two separate keywords: `if` and `while`. This is achieved by checking for the longest matching keyword prefix in any identifier token.

## Token Types

The lexer recognizes the following tokens:

### Keywords

if
while
for
int
char


### Identifiers

[a-zA-Z][a-zA-Z0-9]*


### Numbers

[0-9]+


### Operators

=
+
*
%


## DFA States

The lexical analyzer follows the DFA described in the report:

| State | Meaning |
|------|--------|
| S0 | Start state |
| S1 | Identifier / Keyword |
| S2 | Number |
| S3 | Operator |
| S4 | Error |


## Project Structure

The folder structure is as follows:

mini-lexer/
│
├── lexer.c
├── program.txt
├── Makefile
├── README.md
└── .gitignore


## Compilation

Compile the scanner using:


make


## Running the Lexer

Run the scanner with:


make run


The program reads the input from:


program.txt


and prints the recognized tokens.

## Example Input

ifwhile
forwhile
int count1 = 5


## Example Output

KEYWORD: if
KEYWORD: while
KEYWORD: for
KEYWORD: while
KEYWORD: int
IDENTIFIER: count1
OPERATOR: =
NUMBER: 5
OPERATOR: =
NUMBER: 5
KEYWORD: char
IDENTIFIER: x
OPERATOR: =
NUMBER: 2
IDENTIFIER: count1
OPERATOR: =
IDENTIFIER: count1
OPERATOR: +
NUMBER: 1


## Authors

Group 24 – Compiler Construction

     ODHIAMBO JOHN OTIENO - SCS3/147357/2023
     MICHAEL MUTETI - SCS3/147532/2024
     ABBA BIKO WERE - SCS3/146750/2023
     MARK WAFULA - SCS3/146670/2023




