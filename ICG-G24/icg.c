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
        icg_process_statement(node, ctx);
        return;
    }
    
    /* Otherwise, it should be a STATEMENTS node */
    for (int i = 0; i < node->child_count; i++) {
        ParseTreeNode *child = node->children[i];
        
        if (child->type == NODE_STATEMENT) {
            icg_process_statement(child, ctx);
        } else if (child->type == NODE_STATEMENTS) {
            icg_process_statements(child, ctx);
        }
    }
}

void icg_process_statement(ParseTreeNode *node, ICGContext *ctx) {
    if (!node || !ctx) return;
    
    for (int i = 0; i < node->child_count; i++) {
        ParseTreeNode *child = node->children[i];
        
        switch (child->type) {
            case NODE_DECLARATION:
                icg_process_declaration(child, ctx);
                break;
            case NODE_ASSIGNMENT:
                icg_process_assignment(child, ctx);
                break;
            case NODE_IF_STATEMENT:
                icg_process_if_statement(child, ctx);
                break;
            case NODE_WHILE_STATEMENT:
                icg_process_while_statement(child, ctx);
                break;
            case NODE_FOR_STATEMENT:
                icg_process_for_statement(child, ctx);
                break;
            default:
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
    
    /* Find identifier and expression in children */
    int id_count = 0;
    for (int i = 0; i < node->child_count; i++) {
        ParseTreeNode *child = node->children[i];
        
        if (!child) continue;
        
        if (child->type == NODE_IDENTIFIER) {
            id_count++;
            if (id_count == 1 && var_name[0] == '\0') {
                strncpy(var_name, child->value, 99);
                var_name[99] = '\0';
            }
        } else if (child->type == NODE_EXPRESSION) {
            rhs_result = icg_process_expression(child, ctx);
        }
    }
    
    if (var_name[0] && rhs_result && rhs_result[0]) {
        /* Check if variable was declared */
        if (!symbol_table_exists(ctx->symbol_table, var_name)) {
            symbol_table_insert(ctx->symbol_table, var_name, "int", ctx->scope_level);
        }
        
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
    
    /* Loop label */
    icg_emit_quad(ctx, OP_LABEL, loop_label, "", "");
    
    /* Process condition */
    for (int i = 0; i < node->child_count; i++) {
        if (node->children[i]->type == NODE_EXPRESSION) {
            condition_result = icg_process_expression(node->children[i], ctx);
        }
    }
    
    if (condition_result) {
        /* Jump to exit if condition is false */
        icg_emit_quad(ctx, OP_IF_GOTO, condition_result, "0", exit_label);
        
        /* Process loop body */
        for (int i = 0; i < node->child_count; i++) {
            if (node->children[i]->type == NODE_STATEMENTS ||
                node->children[i]->type == NODE_STATEMENT) {
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
    
    /* For loop structure mirrors while: for <condition> <body>
       Parser produces: ForStatement → 'for' Expression Statement
       which gives the same NODE_EXPRESSION + NODE_STATEMENT children
       as a WhileStatement, so we reuse the same emission logic. */
    icg_process_while_statement(node, ctx);
}

/* ============================================================================
   Expression Processing - Recursive Descent
   ============================================================================ */

/*
 * icg_process_expression walks a NODE_EXPRESSION subtree and returns the
 * name of the variable/temp that holds the result.
 *
 * The parser builds comparison expressions (x < 5) as a flat
 * NODE_EXPRESSION with three children:
 *
 *   Expression
 *     ├─ Expression   (left operand)
 *     ├─ Operator     ("<", ">", "+", etc.)
 *     └─ Expression   (right operand)
 *
 * The original code only handled NODE_EXPRESSION and NODE_TERM children,
 * silently skipping NODE_OPERATOR.  This caused the LHS and operator to
 * be lost: `x < 5` returned "5" instead of emitting `t0 = x < 5`.
 *
 * Fix: track a pending operator whenever we encounter NODE_OPERATOR, then
 * emit a binary quad the next time we resolve a child expression.
 */
char* icg_process_expression(ParseTreeNode *node, ICGContext *ctx) {
    if (!node || !ctx) return "";
    
    /* Leaf cases */
    if (node->type == NODE_NUMBER) {
        return expr_copy_string(node->value);
    }
    
    if (node->type == NODE_IDENTIFIER) {
        return expr_copy_string(node->value);
    }
    
    if (node->type == NODE_EXPRESSION && node->child_count > 0) {
        char *result      = NULL;
        char *pending_op  = NULL;   /* operator waiting for its right operand */
        
        for (int i = 0; i < node->child_count; i++) {
            ParseTreeNode *child = node->children[i];
            
            if (child->type == NODE_OPERATOR) {
                /* Save the operator; the next resolved child is the RHS */
                pending_op = child->value;

            } else if (child->type == NODE_EXPRESSION) {
                char *val = icg_process_expression(child, ctx);
                if (!val || !val[0]) continue;

                if (result && pending_op) {
                    /* Emit:  temp = result <op> val  */
                    char *temp = icg_get_temp_var(ctx);
                    icg_emit_quad(ctx, icg_get_operator_type(pending_op),
                                  result, val, temp);
                    result     = expr_copy_string(temp);
                    pending_op = NULL;
                } else {
                    result = val;
                }

            } else if (child->type == NODE_TERM) {
                /* Pass accumulated result as left operand into the term */
                char *val = icg_process_term(child, ctx, result);
                if (val && val[0]) {
                    result = val;
                }
            }
        }
        
        return (result && result[0]) ? result : "";
    }
    
    if (node->type == NODE_TERM && node->child_count > 0) {
        return icg_process_term(node, ctx, NULL);
    }
    
    if (node->type == NODE_FACTOR && node->child_count > 0) {
        return icg_process_factor(node, ctx);
    }
    
    /* Default: return empty */
    return "";
}

char* icg_process_term(ParseTreeNode *node, ICGContext *ctx, char *left_operand) {
    if (!node || !ctx) return "";
    
    /* Base cases */
    if (node->type == NODE_NUMBER) {
        return expr_copy_string(node->value);
    }
    
    if (node->type == NODE_IDENTIFIER) {
        return expr_copy_string(node->value);
    }
    
    if (node->type == NODE_FACTOR && node->child_count > 0) {
        return icg_process_factor(node, ctx);
    }
    
    if (node->type == NODE_EPSILON) {
        return left_operand ? expr_copy_string(left_operand) : "";
    }
    
    if (node->child_count == 0) {
        return expr_copy_string(node->value);
    }
    
    /* If first child is an operator, this is an AddExpr/MulExpr tail node.
       Use the provided left_operand as the LHS. */
    if (node->children[0]->type == NODE_OPERATOR) {
        char *left = left_operand;
        
        for (int i = 0; i < node->child_count; i++) {
            ParseTreeNode *child = node->children[i];
            
            if (child->type == NODE_EPSILON) {
                continue;
            } else if (child->type == NODE_OPERATOR) {
                char *op_str = child->value;
                OperationType op_type = icg_get_operator_type(op_str);
                
                /* Next child is the right operand */
                i++;
                char *right = NULL;
                if (i < node->child_count) {
                    ParseTreeNode *right_node = node->children[i];
                    if (right_node->type == NODE_FACTOR) {
                        right = icg_process_factor(right_node, ctx);
                    } else if (right_node->type == NODE_TERM) {
                        right = icg_process_term(right_node, ctx, NULL);
                    } else if (right_node->type == NODE_EXPRESSION) {
                        right = icg_process_expression(right_node, ctx);
                    }
                }
                
                if (left && left[0] && right && right[0]) {
                    char *temp = icg_get_temp_var(ctx);
                    icg_emit_quad(ctx, op_type, left, right, temp);
                    left = expr_copy_string(temp);
                }
            }
        }
        
        return left ? (left[0] ? left : (left_operand ? expr_copy_string(left_operand) : "")) : "";
    }
    
    /* Normal term: Term → Factor MulExpr */
    char *left = NULL;
    
    if (node->children[0]->type == NODE_FACTOR) {
        left = icg_process_factor(node->children[0], ctx);
    } else if (node->children[0]->type == NODE_TERM) {
        left = icg_process_term(node->children[0], ctx, NULL);
    } else if (node->children[0]->type != NODE_EPSILON) {
        left = icg_process_expression(node->children[0], ctx);
    }
    
    /* Process remaining children (MulExpr or AddExpr tail nodes) */
    for (int i = 1; i < node->child_count; i++) {
        ParseTreeNode *child = node->children[i];
        
        if (child->type == NODE_EPSILON) {
            continue;
        }
        
        if (child->type == NODE_TERM) {
            left = icg_process_term(child, ctx, left);
        } else if (child->type == NODE_FACTOR) {
            char *right = icg_process_factor(child, ctx);
            if (left && left[0] && right && right[0]) {
                char *temp = icg_get_temp_var(ctx);
                icg_emit_quad(ctx, OP_MUL, left, right, temp);
                left = expr_copy_string(temp);
            }
        }
    }
    
    if (left && left[0]) {
        return left;
    }
    
    return left_operand ? expr_copy_string(left_operand) : "";
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