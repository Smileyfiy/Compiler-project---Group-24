/*
Parse Tree (AST) Implementation
Provides functions for creating, manipulating, and displaying the abstract syntax tree
*/

#include "parse_tree.h"
#include <string.h>

/* Create a new AST node */
ParseTreeNode* ast_create_node(NodeType type, const char *value) {
    ParseTreeNode *node = (ParseTreeNode *)malloc(sizeof(ParseTreeNode));
    if (!node) return NULL;
    
    node->type = type;
    node->capacity = 10;
    node->child_count = 0;
    node->children = (ParseTreeNode **)malloc(sizeof(ParseTreeNode *) * node->capacity);
    
    if (value) {
        strncpy(node->value, value, 99);
        node->value[99] = '\0';
    } else {
        node->value[0] = '\0';
    }
    
    return node;
}

/* Add a child node to a parent */
void ast_add_child(ParseTreeNode *parent, ParseTreeNode *child) {
    if (!parent || !child) return;
    
    /* Expand capacity if needed */
    if (parent->child_count >= parent->capacity) {
        parent->capacity *= 2;
        parent->children = (ParseTreeNode **)realloc(parent->children, 
                                                      sizeof(ParseTreeNode *) * parent->capacity);
    }
    
    parent->children[parent->child_count++] = child;
}

/* Convert node type to string for display */
char* ast_node_type_to_string(NodeType type) {
    switch (type) {
        case NODE_PROGRAM: return "Program";
        case NODE_STATEMENTS: return "Statements";
        case NODE_STATEMENT: return "Statement";
        case NODE_DECLARATION: return "Declaration";
        case NODE_ASSIGNMENT: return "Assignment";
        case NODE_IF_STATEMENT: return "IfStatement";
        case NODE_WHILE_STATEMENT: return "WhileStatement";
        case NODE_FOR_STATEMENT: return "ForStatement";
        case NODE_EXPRESSION: return "Expression";
        case NODE_TERM: return "Term";
        case NODE_FACTOR: return "Factor";
        case NODE_OPERATOR: return "Operator";
        case NODE_IDENTIFIER: return "Identifier";
        case NODE_NUMBER: return "Number";
        case NODE_KEYWORD: return "Keyword";
        case NODE_EPSILON: return "ε";
        default: return "Unknown";
    }
}

/* Print parse tree with indentation */
void ast_print_tree(ParseTreeNode *node, int depth) {
    ast_print_tree_file(node, depth, stdout);
}

/* Print parse tree to file */
void ast_print_tree_file(ParseTreeNode *node, int depth, FILE *f) {
    if (!node) return;
    
    /* Print indentation */
    for (int i = 0; i < depth; i++) {
        fprintf(f, "  ");
    }
    
    /* Print node type */
    fprintf(f, "├─ %s", ast_node_type_to_string(node->type));
    
    /* Print value if present */
    if (strlen(node->value) > 0) {
        fprintf(f, " : \"%s\"", node->value);
    }
    
    fprintf(f, "\n");
    
    /* Print children */
    for (int i = 0; i < node->child_count; i++) {
        ast_print_tree_file(node->children[i], depth + 1, f);
    }
}

/* Free parse tree memory */
void ast_free_tree(ParseTreeNode *node) {
    if (!node) return;
    
    /* Recursively free all children */
    for (int i = 0; i < node->child_count; i++) {
        ast_free_tree(node->children[i]);
    }
    
    /* Free children array and node */
    free(node->children);
    free(node);
}
