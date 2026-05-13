/*
ICG Main Driver
Group 24
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "icg.h"
#include "../Parser-G24/parse_tree.h"
#include "../Parser-G24/parser.h"

int main(int argc, char *argv[]) {
    FILE *input_file = NULL;
    FILE *output_file = NULL;
    ParseTreeNode *parse_tree = NULL;
    ICGContext *icg_ctx = NULL;
    Parser *parser = NULL;
    
    /* Default input file */
    const char *input_filename = "../program.txt";
    const char *output_filename = "icg_output.txt";
    
    /* Parse command line arguments */
    if (argc > 1) {
        input_filename = argv[1];
    }
    if (argc > 2) {
        output_filename = argv[2];
    }
    
    /* Open input file */
    input_file = fopen(input_filename, "r");
    if (!input_file) {
        fprintf(stderr, "ERROR: Cannot open input file '%s'\n", input_filename);
        return 1;
    }
    
    /* Open output file */
    output_file = fopen(output_filename, "w");
    if (!output_file) {
        fprintf(stderr, "ERROR: Cannot open output file '%s'\n", output_filename);
        fclose(input_file);
        return 1;
    }
    
    /* Initialize and run parser */
    parser = parser_init(input_file);
    if (!parser) {
        fprintf(stderr, "ERROR: Parser initialization failed\n");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }
    
    parse_tree = parser_parse(parser);
    fclose(input_file);
    
    if (!parse_tree) {
        fprintf(stderr, "ERROR: Parsing failed\n");
        if (parser) parser_destroy(parser);
        fclose(output_file);
        return 1;
    }
    
    /* Initialize ICG context */
    icg_ctx = icg_init();
    if (!icg_ctx) {
        fprintf(stderr, "ERROR: ICG initialization failed\n");
        fclose(output_file);
        return 1;
    }
    
    /* Generate intermediate code */
    icg_generate(parse_tree, icg_ctx);
    
    /* Print results to console and file */
    fprintf(stdout, "\n========== INTERMEDIATE CODE GENERATION ==========\n");
    fprintf(stdout, "Input: %s\n", input_filename);
    fprintf(stdout, "Output: %s\n\n", output_filename);
    
    quad_list_print(icg_ctx->quad_list, stdout);
    quad_list_print(icg_ctx->quad_list, output_file);
    
    
    fprintf(stdout, "\n");
    fprintf(output_file, "\n");
    
    /* Cleanup */
    ast_free_tree(parse_tree);
    icg_free(icg_ctx);
    if (parser) parser_destroy(parser);
    fclose(output_file);
    
    printf("ICG completed successfully. Output written to: %s\n", output_filename);
    
    return 0;
}