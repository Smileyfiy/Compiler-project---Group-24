/*
Intermediate Code Generation (ICG) Implementation
Group 24
*/

#include "icg.h"

/* ============================================================================
   Buffer Management for Expression Processing
   ============================================================================ */

#define EXPR_BUFFER_POOL_SIZE 20
#define EXPR_BUFFER_SIZE 100

typedef struct {
    char buffers[EXPR_BUFFER_POOL_SIZE][EXPR_BUFFER_SIZE];
    int current_index;
} ExprBufferPool;

static ExprBufferPool expr_pool = {{}, 0};

char* expr_get_buffer() {
    char *buf = expr_pool.buffers[expr_pool.current_index];
    expr_pool.current_index = (expr_pool.current_index + 1) % EXPR_BUFFER_POOL_SIZE;
    buf[0] = '\0';
    return buf;
}

char* expr_copy_string(const char *src) {
    char *dest = expr_get_buffer();
    if (src) {
        strncpy(dest, src, EXPR_BUFFER_SIZE - 1);
        dest[EXPR_BUFFER_SIZE - 1] = '\0';
    }
    return dest;
}

/* ============================================================================
   Context Management
   ============================================================================ */

ICGContext* icg_init() {
    ICGContext *ctx = (ICGContext *)malloc(sizeof(ICGContext));
    if (!ctx) return NULL;
    
    ctx->quad_list = quad_list_init();
    ctx->symbol_table = symbol_table_init();
    ctx->temp_var_count = 0;
    ctx->label_count = 0;
    ctx->scope_level = 0;
    
    return ctx;
}

void icg_free(ICGContext *ctx) {
    if (!ctx) return;
    quad_list_free(ctx->quad_list);
    symbol_table_free(ctx->symbol_table);
    free(ctx);
}

/* ============================================================================
   Symbol Table Management
   ============================================================================ */

SymbolTable* symbol_table_init() {
    SymbolTable *st = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (!st) return NULL;
    
    st->capacity = 100;
    st->count = 0;
    st->symbols = (Symbol *)malloc(st->capacity * sizeof(Symbol));
    
    if (!st->symbols) {
        free(st);
        return NULL;
    }
    
    return st;
}

void symbol_table_free(SymbolTable *st) {
    if (!st) return;
    free(st->symbols);
    free(st);
}

void symbol_table_insert(SymbolTable *st, const char *name, const char *type, int scope) {
    if (!st || st->count >= st->capacity) return;
    
    /* Check if symbol already exists in current scope */
    for (int i = 0; i < st->count; i++) {
        if (strcmp(st->symbols[i].name, name) == 0 && 
            st->symbols[i].scope_level == scope) {
            return;  /* Already exists */
        }
    }
    
    Symbol sym;
    strncpy(sym.name, name, 99);
    sym.name[99] = '\0';
    strncpy(sym.type, type, 49);
    sym.type[49] = '\0';
    sym.scope_level = scope;
    sym.initialized = 0;
    
    st->symbols[st->count++] = sym;
}

Symbol* symbol_table_lookup(SymbolTable *st, const char *name) {
    if (!st) return NULL;
    
    /* Search from highest scope level down */
    for (int i = st->count - 1; i >= 0; i--) {
        if (strcmp(st->symbols[i].name, name) == 0) {
            return &st->symbols[i];
        }
    }
    
    return NULL;
}

int symbol_table_exists(SymbolTable *st, const char *name) {
    return symbol_table_lookup(st, name) != NULL;
}

/* ============================================================================
   Temporary Variables and Labels
   ============================================================================ */

char* icg_get_temp_var(ICGContext *ctx) {
    if (!ctx) return "";
    
    char temp_str[50];
    sprintf(temp_str, "t%d", ctx->temp_var_count++);
    return expr_copy_string(temp_str);
}

char* icg_get_label(ICGContext *ctx) {
    if (!ctx) return "";
    
    char label_str[50];
    sprintf(label_str, "L%d", ctx->label_count++);
    return expr_copy_string(label_str);
}

/* ============================================================================
   Quadruple List Management
   ============================================================================ */

QuadrupleList* quad_list_init() {
    QuadrupleList *ql = (QuadrupleList *)malloc(sizeof(QuadrupleList));
    if (!ql) return NULL;
    
    ql->capacity = 500;
    ql->count = 0;
    ql->quads = (Quadruple *)malloc(ql->capacity * sizeof(Quadruple));
    
    if (!ql->quads) {
        free(ql);
        return NULL;
    }
    
    return ql;
}

void quad_list_free(QuadrupleList *ql) {
    if (!ql) return;
    free(ql->quads);
    free(ql);
}

void quad_list_add(QuadrupleList *ql, Quadruple quad) {
    if (!ql) return;
    
    /* Resize if needed */
    if (ql->count >= ql->capacity) {
        ql->capacity *= 2;
        ql->quads = (Quadruple *)realloc(ql->quads, ql->capacity * sizeof(Quadruple));
    }
    
    ql->quads[ql->count++] = quad;
}

void quad_list_print(QuadrupleList *ql, FILE *f) {
    if (!ql || !f) return;
    
    fprintf(f, "\n========== INTERMEDIATE CODE (QUADRUPLES) ==========\n");
    fprintf(f, "%-5s | %-15s | %-15s | %-15s | %-15s\n",
            "NUM", "OP", "ARG1", "ARG2", "RESULT");
    fprintf(f, "------|-----------------|-----------------|-----------------|------------------\n");
    
    for (int i = 0; i < ql->count; i++) {
        Quadruple q = ql->quads[i];
        fprintf(f, "%-5d | %-15s | %-15s | %-15s | %-15s\n",
                i, 
                icg_operator_to_string(q.op),
                q.arg1[0] ? q.arg1 : "-",
                q.arg2[0] ? q.arg2 : "-",
                q.result[0] ? q.result : "-");
    }
    fprintf(f, "\n");
}

/* ============================================================================
   Operator Handling
   ============================================================================ */

OperationType icg_get_operator_type(const char *op_str) {
    if (!op_str) return OP_ASSIGN;
    
    /* Only operators recognized by Lexer-G24 */
    if (strcmp(op_str, "+") == 0) return OP_ADD;
    if (strcmp(op_str, "-") == 0) return OP_SUB;
    if (strcmp(op_str, "*") == 0) return OP_MUL;
    if (strcmp(op_str, "%") == 0) return OP_MOD;
    if (strcmp(op_str, "<") == 0) return OP_LT;
    if (strcmp(op_str, ">") == 0) return OP_GT;
    
    return OP_ASSIGN;
}

const char* icg_operator_to_string(OperationType op) {
    switch (op) {
        case OP_ASSIGN:     return "=";
        case OP_ADD:        return "+";
        case OP_SUB:        return "-";
        case OP_MUL:        return "*";
        case OP_MOD:        return "%";
        case OP_LT:         return "<";
        case OP_GT:         return ">";
        case OP_LABEL:      return "label";
        case OP_GOTO:       return "goto";
        case OP_IF_GOTO:    return "if_goto";
        case OP_DECLARE:    return "declare";
        case OP_END:        return "end";
        default:            return "unknown";
    }
}

void icg_emit_quad(ICGContext *ctx, OperationType op, const char *arg1,
                   const char *arg2, const char *result) {
    if (!ctx) return;
    
    Quadruple quad;
    quad.op = op;
    
    strncpy(quad.arg1, arg1 ? arg1 : "", 99);
    quad.arg1[99] = '\0';
    
    strncpy(quad.arg2, arg2 ? arg2 : "", 99);
    quad.arg2[99] = '\0';
    
    strncpy(quad.result, result ? result : "", 99);
    quad.result[99] = '\0';
    
    quad_list_add(ctx->quad_list, quad);
}

/* ============================================================================
   AST Processing - Main Entry Point
   ============================================================================ */

void icg_generate(ParseTreeNode *ast, ICGContext *ctx) {
    if (!ast || !ctx) return;
    
    if (ast->type == NODE_PROGRAM) {
        icg_process_program(ast, ctx);
    }
    
    icg_emit_quad(ctx, OP_END, "", "", "");
}

void icg_process_program(ParseTreeNode *node, ICGContext *ctx) {
    if (!node || !ctx) return;
    
    for (int i = 0; i < node->child_count; i++) {
        if (node->children[i]->type == NODE_STATEMENTS) {
            icg_process_statements(node->children[i], ctx);
        }
    }
}

void icg_process_statements(ParseTreeNode *node, ICGContext *ctx) {
    if (!node || !ctx) return;
    
    /* If this is a STATEMENT node, process it directly */
    if (node->type == NODE_STATEMENT) {
        fprintf(stderr, "[DEBUG STMTS] Processing Statement node\n");
        icg_process_statement(node, ctx);
        return;
    }
    
    /* Otherwise, it should be a STATEMENTS node */
    fprintf(stderr, "[DEBUG STMTS] Processing Statements node with %d children\n", node->child_count);
    
    for (int i = 0; i < node->child_count; i++) {
        ParseTreeNode *child = node->children[i];
        
        if (child->type == NODE_STATEMENT) {
            fprintf(stderr, "[DEBUG STMTS] Found Statement child\n");
            icg_process_statement(child, ctx);
        } else if (child->type == NODE_STATEMENTS) {
            fprintf(stderr, "[DEBUG STMTS] Found Statements child, recursing\n");
            icg_process_statements(child, ctx);
        }
    }
}

void icg_process_statement(ParseTreeNode *node, ICGContext *ctx) {
    if (!node || !ctx) return;
    
    fprintf(stderr, "[DEBUG STMT] Processing statement with %d children\n", node->child_count);
    
    for (int i = 0; i < node->child_count; i++) {
        ParseTreeNode *child = node->children[i];
        
        fprintf(stderr, "[DEBUG STMT] Child %d type: %d\n", i, child->type);
        
        switch (child->type) {
            case NODE_DECLARATION:
                fprintf(stderr, "[DEBUG STMT] -> Declaration\n");
                icg_process_declaration(child, ctx);
                break;
            case NODE_ASSIGNMENT:
                fprintf(stderr, "[DEBUG STMT] -> Assignment\n");
                icg_process_assignment(child, ctx);
                break;
            case NODE_IF_STATEMENT:
                fprintf(stderr, "[DEBUG STMT] -> If\n");
                icg_process_if_statement(child, ctx);
                break;
            case NODE_WHILE_STATEMENT:
                fprintf(stderr, "[DEBUG STMT] -> While\n");
                icg_process_while_statement(child, ctx);
                break;
            case NODE_FOR_STATEMENT:
                fprintf(stderr, "[DEBUG STMT] -> For\n");
                icg_process_for_statement(child, ctx);
                break;
            default:
                fprintf(stderr, "[DEBUG STMT] -> Unknown type %d\n", child->type);
                break;
        }
    }
}

void icg_process_declaration(ParseTreeNode *node, ICGContext *ctx) {
    if (!node || !ctx) return;
    
    /* Declaration has form: type identifier */
    char type_name[50] = "int";
    char var_name[100] = "";
    
    /* Traverse all children to find keyword and identifier */
    for (int i = 0; i < node->child_count; i++) {
        ParseTreeNode *child = node->children[i];
        
        if (!child) continue;
        
        if (child->type == NODE_KEYWORD) {
            strncpy(type_name, child->value, 49);
            type_name[49] = '\0';
        } else if (child->type == NODE_IDENTIFIER) {
            strncpy(var_name, child->value, 99);
            var_name[99] = '\0';
        }
    }
    
    if (var_name[0]) {
        symbol_table_insert(ctx->symbol_table, var_name, type_name, ctx->scope_level);
        icg_emit_quad(ctx, OP_DECLARE, type_name, "", var_name);
    }
}

void icg_process_assignment(ParseTreeNode *node, ICGContext *ctx) {
    if (!node || !ctx) return;
    
    char var_name[100] = "";
    char *rhs_result = NULL;
    
    fprintf(stderr, "[DEBUG ASSIGN] Processing assignment with %d children\n", node->child_count);
    
    /* Find identifier and expression in children */
    int id_count = 0;
    for (int i = 0; i < node->child_count; i++) {
        ParseTreeNode *child = node->children[i];
        
        if (!child) continue;
        
        fprintf(stderr, "[DEBUG ASSIGN] Child %d type: %d, value: %s\n", i, child->type, child->value);
        
        if (child->type == NODE_IDENTIFIER) {
            id_count++;
            if (id_count == 1 && var_name[0] == '\0') {
                strncpy(var_name, child->value, 99);
                var_name[99] = '\0';
                fprintf(stderr, "[DEBUG ASSIGN] Found first identifier: %s\n", var_name);
            } else {
                fprintf(stderr, "[DEBUG ASSIGN] Found identifier #%d: %s\n", id_count, child->value);
            }
        } else if (child->type == NODE_EXPRESSION) {
            rhs_result = icg_process_expression(child, ctx);
            fprintf(stderr, "[DEBUG ASSIGN] Found expression result: %s\n", rhs_result);
        }
    }
    
    fprintf(stderr, "[DEBUG ASSIGN] Final: var_name=%s, rhs_result=%s\n", var_name, rhs_result ? rhs_result : "(null)");
    
    if (var_name[0] && rhs_result && rhs_result[0]) {
        /* Check if variable was declared */
        if (!symbol_table_exists(ctx->symbol_table, var_name)) {
            symbol_table_insert(ctx->symbol_table, var_name, "int", ctx->scope_level);
        }
        
        fprintf(stderr, "[DEBUG ASSIGN] Emitting quad: %s = %s\n", var_name, rhs_result);
        icg_emit_quad(ctx, OP_ASSIGN, rhs_result, "", var_name);
    }
}

void icg_process_if_statement(ParseTreeNode *node, ICGContext *ctx) {
    if (!node || !ctx) return;
    
    char *condition_result = NULL;
    char *else_label = NULL;
    char *end_label = NULL;
    
    for (int i = 0; i < node->child_count; i++) {
        ParseTreeNode *child = node->children[i];
        
        if (child->type == NODE_EXPRESSION) {
            condition_result = icg_process_expression(child, ctx);
        }
    }
    
    if (condition_result) {
        else_label = icg_get_label(ctx);
        end_label = icg_get_label(ctx);
        
        /* Jump to else if condition is false */
        icg_emit_quad(ctx, OP_IF_GOTO, condition_result, "0", else_label);
        
        /* Process if body */
        for (int i = 0; i < node->child_count; i++) {
            if (node->children[i]->type == NODE_STATEMENTS ||
                node->children[i]->type == NODE_STATEMENT) {
                icg_process_statements(node->children[i], ctx);
            }
        }
        
        /* Jump to end */
        icg_emit_quad(ctx, OP_GOTO, end_label, "", "");
        
        /* Else label */
        icg_emit_quad(ctx, OP_LABEL, else_label, "", "");
        
        /* End label */
        icg_emit_quad(ctx, OP_LABEL, end_label, "", "");
    }
}

void icg_process_while_statement(ParseTreeNode *node, ICGContext *ctx) {
    if (!node || !ctx) return;
    
    char *loop_label = icg_get_label(ctx);
    char *exit_label = icg_get_label(ctx);
    char *condition_result = NULL;
    
    fprintf(stderr, "[DEBUG WHILE] Processing while with %d children\n", node->child_count);
    
    /* Loop label */
    icg_emit_quad(ctx, OP_LABEL, loop_label, "", "");
    
    /* Process condition */
    for (int i = 0; i < node->child_count; i++) {
        fprintf(stderr, "[DEBUG WHILE] Child %d type: %d\n", i, node->children[i]->type);
        if (node->children[i]->type == NODE_EXPRESSION) {
            condition_result = icg_process_expression(node->children[i], ctx);
            fprintf(stderr, "[DEBUG WHILE] Found condition expression\n");
        }
    }
    
    if (condition_result) {
        /* Jump to exit if condition is false */
        icg_emit_quad(ctx, OP_IF_GOTO, condition_result, "0", exit_label);
        
        /* Process loop body */
        fprintf(stderr, "[DEBUG WHILE] Processing body with loop_label=%s, exit_label=%s\n", loop_label, exit_label);
        for (int i = 0; i < node->child_count; i++) {
            fprintf(stderr, "[DEBUG WHILE] Body child %d type: %d\n", i, node->children[i]->type);
            if (node->children[i]->type == NODE_STATEMENTS ||
                node->children[i]->type == NODE_STATEMENT) {
                fprintf(stderr, "[DEBUG WHILE] Processing body statement/statements\n");
                icg_process_statements(node->children[i], ctx);
            }
        }
        
        /* Jump back to loop */
        icg_emit_quad(ctx, OP_GOTO, loop_label, "", "");
        
        /* Exit label */
        icg_emit_quad(ctx, OP_LABEL, exit_label, "", "");
    }
}

void icg_process_for_statement(ParseTreeNode *node, ICGContext *ctx) {
    if (!node || !ctx) return;
    
    /* For now, treat for as a while loop */
    /* Future: add proper for loop handling */
    icg_process_while_statement(node, ctx);
}

/* ============================================================================
   Expression Processing - Recursive Descent
   ============================================================================ */

char* icg_process_expression(ParseTreeNode *node, ICGContext *ctx) {
    if (!node || !ctx) return "";
    
    /* Handle different node types */
    if (node->type == NODE_NUMBER) {
        fprintf(stderr, "[DEBUG] Expression->Number: %s\n", node->value);
        return expr_copy_string(node->value);
    }
    
    if (node->type == NODE_IDENTIFIER) {
        fprintf(stderr, "[DEBUG] Expression->Identifier: %s\n", node->value);
        return expr_copy_string(node->value);
    }
    
    if (node->type == NODE_EXPRESSION && node->child_count > 0) {
        fprintf(stderr, "[DEBUG] Expression has %d children\n", node->child_count);
        
        /* Look for binary operation: Expression Operator Expression */
        char *left = NULL;
        char *op_str = NULL;
        char *right = NULL;
        
        for (int i = 0; i < node->child_count; i++) {
            ParseTreeNode *child = node->children[i];
            fprintf(stderr, "[DEBUG] Child %d type: %d\n", i, child->type);
            
            if (child->type == NODE_EXPRESSION && left == NULL) {
                left = icg_process_expression(child, ctx);
                fprintf(stderr, "[DEBUG] Set left from Expression: %s\n", left);
            } else if (child->type == NODE_OPERATOR) {
                op_str = child->value;
                fprintf(stderr, "[DEBUG] Found operator: %s\n", op_str);
            } else if (child->type == NODE_TERM && left == NULL) {
                left = icg_process_term(child, ctx);
                fprintf(stderr, "[DEBUG] Set left from Term: %s\n", left);
            } else if (child->type == NODE_EXPRESSION && op_str != NULL && right == NULL) {
                right = icg_process_expression(child, ctx);
                fprintf(stderr, "[DEBUG] Set right from Expression: %s\n", right);
            } else if (child->type == NODE_TERM && op_str != NULL && right == NULL) {
                right = icg_process_term(child, ctx);
                fprintf(stderr, "[DEBUG] Set right from Term: %s\n", right);
            }
        }
        
        /* If we found a binary operation, emit quad */
        if (left && left[0] && op_str && right && right[0]) {
            fprintf(stderr, "[DEBUG] Emitting binary op: %s %s %s\n", left, op_str, right);
            OperationType op_type = icg_get_operator_type(op_str);
            char *temp = icg_get_temp_var(ctx);
            icg_emit_quad(ctx, op_type, left, right, temp);
            return expr_copy_string(temp);
        }
        
        /* Otherwise just process as term */
        if (left && left[0]) {
            fprintf(stderr, "[DEBUG] Returning left: %s\n", left);
            return left;
        }
        
        /* Or process first child */
        fprintf(stderr, "[DEBUG] Processing first child\n");
        return icg_process_expression(node->children[0], ctx);
    }
    
    if (node->type == NODE_TERM && node->child_count > 0) {
        fprintf(stderr, "[DEBUG] Delegating to process_term\n");
        return icg_process_term(node, ctx);
    }
    
    if (node->type == NODE_FACTOR && node->child_count > 0) {
        fprintf(stderr, "[DEBUG] Delegating to process_factor\n");
        return icg_process_factor(node, ctx);
    }
    
    /* Default: return empty */
    fprintf(stderr, "[DEBUG] Returning empty from expression\n");
    return "";
}

char* icg_process_term(ParseTreeNode *node, ICGContext *ctx) {
    if (!node || !ctx) return "";
    
    /* Base cases */
    if (node->type == NODE_NUMBER) {
        fprintf(stderr, "[DEBUG TERM] Number: %s\n", node->value);
        return expr_copy_string(node->value);
    }
    
    if (node->type == NODE_IDENTIFIER) {
        fprintf(stderr, "[DEBUG TERM] Identifier: %s\n", node->value);
        return expr_copy_string(node->value);
    }
    
    if (node->type == NODE_FACTOR && node->child_count > 0) {
        fprintf(stderr, "[DEBUG TERM] Delegating Factor to process_factor\n");
        return icg_process_factor(node, ctx);
    }
    
    if (node->child_count == 0) {
        fprintf(stderr, "[DEBUG TERM] No children, using value: %s\n", node->value);
        return expr_copy_string(node->value);
    }
    
    fprintf(stderr, "[DEBUG TERM] Term has %d children, processing them\n", node->child_count);
    
    /* Process binary operations: left op right */
    char *left = NULL;
    
    /* First child should be a Factor or Term */
    if (node->children[0]->type == NODE_FACTOR) {
        left = icg_process_factor(node->children[0], ctx);
        fprintf(stderr, "[DEBUG TERM] Got left from Factor: %s\n", left);
    } else if (node->children[0]->type == NODE_TERM) {
        left = icg_process_term(node->children[0], ctx);
        fprintf(stderr, "[DEBUG TERM] Got left from Term: %s\n", left);
    } else {
        left = icg_process_expression(node->children[0], ctx);
        fprintf(stderr, "[DEBUG TERM] Got left from Expression: %s\n", left);
    }
    
    for (int i = 1; i < node->child_count; i += 2) {
        if (i + 1 < node->child_count) {
            ParseTreeNode *op_node = node->children[i];
            ParseTreeNode *right_node = node->children[i + 1];
            char *right = NULL;
            
            if (right_node->type == NODE_FACTOR) {
                right = icg_process_factor(right_node, ctx);
            } else if (right_node->type == NODE_TERM) {
                right = icg_process_term(right_node, ctx);
            } else {
                right = icg_process_expression(right_node, ctx);
            }
            
            OperationType op_type = OP_MUL;  /* Default to multiply for terms */
            if (op_node && op_node->value[0]) {
                op_type = icg_get_operator_type(op_node->value);
            }
            
            char *temp = icg_get_temp_var(ctx);
            icg_emit_quad(ctx, op_type, left, right, temp);
            left = expr_copy_string(temp);
        }
    }
    
    if (left && left[0]) {
        fprintf(stderr, "[DEBUG TERM] Returning: %s\n", left);
        return left;
    }
    
    fprintf(stderr, "[DEBUG TERM] Returning empty\n");
    return "";
}

char* icg_process_factor(ParseTreeNode *node, ICGContext *ctx) {
    if (!node || !ctx) return "";
    
    static char result[100];
    
    if (node->type == NODE_NUMBER) {
        strncpy(result, node->value, 99);
        result[99] = '\0';
        return result;
    }
    
    if (node->type == NODE_IDENTIFIER) {
        strncpy(result, node->value, 99);
        result[99] = '\0';
        return result;
    }
    
    if (node->type == NODE_EXPRESSION) {
        return icg_process_expression(node, ctx);
    }
    
    /* Process node with children */
    if (node->child_count > 0) {
        return icg_process_expression(node->children[0], ctx);
    }
    
    /* Fallback to node value if no children */
    if (node->value[0]) {
        strncpy(result, node->value, 99);
        result[99] = '\0';
        return result;
    }
    
    return "";
}
