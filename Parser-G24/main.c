/*
Main program for LL(1) Parser with Parse Tree Output
*/

#include "parser.h"
#include "parse_tree.h"

int main(void) {
    FILE *input_file;
    Parser *parser;
    ParseTreeNode *parse_tree;
    
    /* Open input file */
    input_file = fopen("program.txt", "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error: Could not open program.txt\n");
        return 1;
    }
    
    /* Initialize parser */
    parser = parser_init(input_file);
    
    /* Parse and get parse tree */
    parse_tree = parser_parse(parser);
    
    if (parse_tree) {
        printf("\n\n=== PARSE TREE ===\n\n");
        ast_print_tree(parse_tree, 0);
        printf("\n");
        
        /* Free the parse tree */
        ast_free_tree(parse_tree);
    }
    
    /* Cleanup */
    parser_destroy(parser);
    fclose(input_file);
    
    return (parse_tree != NULL) ? 0 : 1;
}
