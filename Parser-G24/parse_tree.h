#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <stdlib.h>
#include <stdio.h>

/* AST Node types */
typedef enum {
    NODE_PROGRAM,
    NODE_STATEMENTS,
    NODE_STATEMENT,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_IF_STATEMENT,
    NODE_WHILE_STATEMENT,
    NODE_FOR_STATEMENT,
    NODE_EXPRESSION,
    NODE_TERM,
    NODE_FACTOR,
    NODE_OPERATOR,
    NODE_IDENTIFIER,
    NODE_NUMBER,
    NODE_KEYWORD,
    NODE_EPSILON  /* For empty productions */
} NodeType;

/* AST Node structure */
typedef struct ParseTreeNode {
    NodeType type;
    char value[100];           /* Token value (identifier, number, keyword) */
    struct ParseTreeNode **children;
    int child_count;
    int capacity;
} ParseTreeNode;

/* Function declarations */
ParseTreeNode* ast_create_node(NodeType type, const char *value);
void ast_add_child(ParseTreeNode *parent, ParseTreeNode *child);
void ast_print_tree(ParseTreeNode *node, int depth);
void ast_print_tree_file(ParseTreeNode *node, int depth, FILE *f);
void ast_free_tree(ParseTreeNode *node);
char* ast_node_type_to_string(NodeType type);

#endif
