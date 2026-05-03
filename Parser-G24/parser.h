/*
LL(1) Parser Header
*/

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_tree.h"

/* Token types */
typedef enum {
    TOK_EOF = 0,
    TOK_KEYWORD_IF = 1,
    TOK_KEYWORD_WHILE = 2,
    TOK_KEYWORD_FOR = 3,
    TOK_KEYWORD_INT = 4,
    TOK_KEYWORD_CHAR = 5,
    TOK_IDENTIFIER = 6,
    TOK_NUMBER = 7,
    TOK_ASSIGN = 8,      /* = */
    TOK_PLUS = 9,        /* + */
    TOK_MUL = 10,        /* * */
    TOK_MOD = 11,        /* % */
    TOK_LT = 13,         /* < */
    TOK_GT = 14,         /* > */
    TOK_ERROR = 12
} TokenType;

typedef struct {
    TokenType type;
    char value[100];
} Token;

/* Non-terminals for parsing table */
typedef enum {
    NT_PROGRAM = 0,
    NT_STATEMENTS = 1,
    NT_STATEMENT = 2,
    NT_DECLARATION = 3,
    NT_ASSIGNMENT = 4,
    NT_IF_STATEMENT = 5,
    NT_WHILE_STATEMENT = 6,
    NT_FOR_STATEMENT = 7,
    NT_EXPRESSION = 8,
    NT_ADD_EXPR = 9,
    NT_TERM = 10,
    NT_MUL_EXPR = 11,
    NT_FACTOR = 12
} NonTerminal;

/* Parse action types */
typedef enum {
    ACT_ACCEPT = 0,
    ACT_REDUCE = 1,
    ACT_SHIFT = 2,
    ACT_ERROR = 3,
    ACT_EPSILON = 4
} ActionType;

/* Production rule structure */
typedef struct {
    NonTerminal lhs;
    int rhs_count;
    NonTerminal rhs[5];
} Production;

/* Parser state */
typedef struct {
    Token current_token;
    FILE *input_file;
    int error_count;
    int line_number;
    char error_messages[100][200];  /* Store error messages */
} Parser;

/* Function declarations */
Parser* parser_init(FILE *input);
void parser_destroy(Parser *parser);
Token parser_next_token(Parser *parser);
ParseTreeNode* parser_parse(Parser *parser);
void parser_print_error(Parser *parser, const char *message);

#endif
