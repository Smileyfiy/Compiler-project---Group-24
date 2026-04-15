/*
Main program for LL(1) Parser
*/

#include "parser.h"

int main(void) {
    FILE *input_file;
    Parser *parser;
    int result;
    
    /* Open input file */
    input_file = fopen("program.txt", "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error: Could not open program.txt\n");
        return 1;
    }
    
    /* Initialize parser */
    parser = parser_init(input_file);
    
    /* Parse */
    result = parser_parse(parser);
    
    /* Cleanup */
    parser_destroy(parser);
    fclose(input_file);
    
    return result ? 0 : 1;
}
