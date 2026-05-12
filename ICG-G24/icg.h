/*
Intermediate Code Generation (ICG) using Quadruples
Group 24
*/

#ifndef ICG_H
#define ICG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Parser-G24/parse_tree.h"

/* Quadruple Operation Types */
/* Only operators recognized by Lexer-G24: =, +, -, *, %, <, > */
typedef enum {
    OP_ASSIGN,      /* = */
    OP_ADD,         /* + */
    OP_SUB,         /* - */
    OP_MUL,         /* * */
    OP_MOD,         /* % */
    OP_LT,          /* < */
    OP_GT,          /* > */
    OP_LABEL,       /* Label (generated internally) */
    OP_GOTO,        /* Unconditional jump (generated internally) */
    OP_IF_GOTO,     /* Conditional jump (generated internally) */
    OP_DECLARE,     /* Variable declaration (generated internally) */
    OP_END          /* Program end (generated internally) */
} OperationType;

/* Symbol Table Entry */
typedef struct {
    char name[100];
    char type[50];      /* int, char, etc. */
    int scope_level;
    int initialized;
} Symbol;

/* Symbol Table */
typedef struct {
    Symbol *symbols;
    int count;
    int capacity;
} SymbolTable;

/* Quadruple Structure */
typedef struct {
    OperationType op;           /* Operation type */
    char arg1[100];            /* First argument */
    char arg2[100];            /* Second argument */
    char result[100];          /* Result/destination */
} Quadruple;

/* Quadruple List */
typedef struct {
    Quadruple *quads;
    int count;
    int capacity;
} QuadrupleList;

/* ICG Context */
typedef struct {
    QuadrupleList *quad_list;
    SymbolTable *symbol_table;
    int temp_var_count;        /* Counter for temporary variables */
    int label_count;           /* Counter for labels */
    int scope_level;           /* Current scope depth */
} ICGContext;

/* Function Declarations */

/* Context Management */
ICGContext* icg_init();
void icg_free(ICGContext *ctx);

/* Symbol Table Management */
SymbolTable* symbol_table_init();
void symbol_table_free(SymbolTable *st);
void symbol_table_insert(SymbolTable *st, const char *name, const char *type, int scope);
Symbol* symbol_table_lookup(SymbolTable *st, const char *name);
int symbol_table_exists(SymbolTable *st, const char *name);

/* Temporary Variable and Label Generation */
char* icg_get_temp_var(ICGContext *ctx);
char* icg_get_label(ICGContext *ctx);

/* Quadruple Management */
QuadrupleList* quad_list_init();
void quad_list_free(QuadrupleList *ql);
void quad_list_add(QuadrupleList *ql, Quadruple quad);
void quad_list_print(QuadrupleList *ql, FILE *f);

/* ICG Generation from AST */
void icg_generate(ParseTreeNode *ast, ICGContext *ctx);

/* AST Node Processing */
void icg_process_program(ParseTreeNode *node, ICGContext *ctx);
void icg_process_statements(ParseTreeNode *node, ICGContext *ctx);
void icg_process_statement(ParseTreeNode *node, ICGContext *ctx);
void icg_process_declaration(ParseTreeNode *node, ICGContext *ctx);
void icg_process_assignment(ParseTreeNode *node, ICGContext *ctx);
void icg_process_if_statement(ParseTreeNode *node, ICGContext *ctx);
void icg_process_while_statement(ParseTreeNode *node, ICGContext *ctx);
void icg_process_for_statement(ParseTreeNode *node, ICGContext *ctx);
char* icg_process_expression(ParseTreeNode *node, ICGContext *ctx);
char* icg_process_term(ParseTreeNode *node, ICGContext *ctx, char *left_operand);
char* icg_process_factor(ParseTreeNode *node, ICGContext *ctx);

/* Utility Functions */
void icg_emit_quad(ICGContext *ctx, OperationType op, const char *arg1, 
                   const char *arg2, const char *result);
OperationType icg_get_operator_type(const char *op_str);
const char* icg_operator_to_string(OperationType op);

#endif