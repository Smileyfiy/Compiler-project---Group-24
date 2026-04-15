/*
LL(1) Parser for Mini-Grammar
Uses table-driven parsing with tokens from lexer

Group 24 Members:
     ODHIAMBO JOHN OTIENO - SCS3/147357/2023
     MICHAEL MUTETI - SCS3/147532/2024
     ABBA BIKO WERE - SCS3/146750/2023
     MARK WAFULA - SCS3/146670/2023
*/

#include "parser.h"
#include <ctype.h>

/* Production rules (indexed by production number)
   Note: Currently documented for reference; recursive descent parser uses function-based rules */
static const Production productions[] __attribute__((unused)) = {
    {NT_PROGRAM, 1, {NT_STATEMENTS}},                                    /* 0: Program → Statements */
    {NT_STATEMENTS, 2, {NT_STATEMENT, NT_STATEMENTS}},                    /* 1: Statements → Statement Statements */
    {NT_STATEMENTS, 0, {}},                                               /* 2: Statements → ε */
    {NT_STATEMENT, 1, {NT_DECLARATION}},                                  /* 3: Statement → Declaration */
    {NT_STATEMENT, 1, {NT_ASSIGNMENT}},                                   /* 4: Statement → Assignment */
    {NT_STATEMENT, 1, {NT_IF_STATEMENT}},                                 /* 5: Statement → IfStatement */
    {NT_STATEMENT, 1, {NT_WHILE_STATEMENT}},                              /* 6: Statement → WhileStatement */
    {NT_STATEMENT, 1, {NT_FOR_STATEMENT}},                                /* 7: Statement → ForStatement */
    {NT_DECLARATION, 2, {-1, TOK_KEYWORD_INT}},                           /* 8: Declaration → int IDENTIFIER (special handling) */
    {NT_DECLARATION, 2, {-1, TOK_KEYWORD_CHAR}},                          /* 9: Declaration → char IDENTIFIER */
    {NT_ASSIGNMENT, 3, {-1, -1, -1}},                                     /* 10: Assignment → IDENTIFIER = Expression */
    {NT_IF_STATEMENT, 3, {-1, -1, -1}},                                   /* 11: IfStatement → if Expression Statement */
    {NT_WHILE_STATEMENT, 3, {-1, -1, -1}},                                /* 12: WhileStatement → while Expression Statement */
    {NT_FOR_STATEMENT, 3, {-1, -1, -1}},                                  /* 13: ForStatement → for Assignment Statement */
    {NT_EXPRESSION, 2, {NT_TERM, NT_ADD_EXPR}},                           /* 14: Expression → Term AddExpr */
    {NT_ADD_EXPR, 3, {-1, NT_TERM, NT_ADD_EXPR}},                         /* 15: AddExpr → + Term AddExpr */
    {NT_ADD_EXPR, 0, {}},                                                 /* 16: AddExpr → ε */
    {NT_TERM, 2, {NT_FACTOR, NT_MUL_EXPR}},                               /* 17: Term → Factor MulExpr */
    {NT_MUL_EXPR, 3, {-1, NT_FACTOR, NT_MUL_EXPR}},                       /* 18: MulExpr → * Factor MulExpr */
    {NT_MUL_EXPR, 3, {-1, NT_FACTOR, NT_MUL_EXPR}},                       /* 19: MulExpr → % Factor MulExpr */
    {NT_MUL_EXPR, 0, {}},                                                 /* 20: MulExpr → ε */
    {NT_FACTOR, 1, {-1}},                                                 /* 21: Factor → IDENTIFIER */
    {NT_FACTOR, 1, {-1}}                                                  /* 22: Factor → NUMBER */
};

/* Parsing strategy - simplified stack-based approach */
#define MAX_STACK 256
#define MAX_PRODUCTIONS 23

/* Lexer interface - reads tokens from file */
static void lexer_read_token(Parser *parser) {
    char ch;
    int i = 0;
    
    /* Skip whitespace */
    while ((ch = fgetc(parser->input_file)) != EOF && isspace(ch));
    
    if (ch == EOF) {
        parser->current_token.type = TOK_EOF;
        strcpy(parser->current_token.value, "EOF");
        return;
    }
    
    /* Check for keywords and identifiers */
    if (isalpha(ch)) {
        parser->current_token.value[i++] = ch;
        while ((ch = fgetc(parser->input_file)) != EOF && isalnum(ch)) {
            parser->current_token.value[i++] = ch;
        }
        parser->current_token.value[i] = '\0';
        
        if (feof(parser->input_file) == 0) {
            ungetc(ch, parser->input_file);
        }
        
        /* Check if keyword */
        if (strcmp(parser->current_token.value, "if") == 0)
            parser->current_token.type = TOK_KEYWORD_IF;
        else if (strcmp(parser->current_token.value, "while") == 0)
            parser->current_token.type = TOK_KEYWORD_WHILE;
        else if (strcmp(parser->current_token.value, "for") == 0)
            parser->current_token.type = TOK_KEYWORD_FOR;
        else if (strcmp(parser->current_token.value, "int") == 0)
            parser->current_token.type = TOK_KEYWORD_INT;
        else if (strcmp(parser->current_token.value, "char") == 0)
            parser->current_token.type = TOK_KEYWORD_CHAR;
        else
            parser->current_token.type = TOK_IDENTIFIER;
        return;
    }
    
    /* Check for numbers */
    if (isdigit(ch)) {
        parser->current_token.value[i++] = ch;
        while ((ch = fgetc(parser->input_file)) != EOF && isdigit(ch)) {
            parser->current_token.value[i++] = ch;
        }
        parser->current_token.value[i] = '\0';
        
        if (feof(parser->input_file) == 0) {
            ungetc(ch, parser->input_file);
        }
        
        parser->current_token.type = TOK_NUMBER;
        return;
    }
    
    /* Check for operators */
    switch (ch) {
        case '=':
            parser->current_token.type = TOK_ASSIGN;
            strcpy(parser->current_token.value, "=");
            break;
        case '+':
            parser->current_token.type = TOK_PLUS;
            strcpy(parser->current_token.value, "+");
            break;
        case '*':
            parser->current_token.type = TOK_MUL;
            strcpy(parser->current_token.value, "*");
            break;
        case '%':
            parser->current_token.type = TOK_MOD;
            strcpy(parser->current_token.value, "%");
            break;
        default:
            parser->current_token.type = TOK_ERROR;
            sprintf(parser->current_token.value, "%c", ch);
    }
}

/* Initialize parser */
Parser* parser_init(FILE *input) {
    Parser *parser = (Parser *)malloc(sizeof(Parser));
    parser->input_file = input;
    parser->error_count = 0;
    lexer_read_token(parser);
    return parser;
}

/* Clean up parser */
void parser_destroy(Parser *parser) {
    if (parser) {
        free(parser);
    }
}

/* Print error message */
void parser_print_error(const char *message) {
    fprintf(stderr, "PARSE ERROR: %s\n", message);
}

/* Recursive descent parser for LL(1) */
static int parse_program(Parser *parser);
static int parse_statements(Parser *parser);
static int parse_statement(Parser *parser);
static int parse_declaration(Parser *parser);
static int parse_assignment(Parser *parser);
static int parse_if_statement(Parser *parser);
static int parse_while_statement(Parser *parser);
static int parse_for_statement(Parser *parser);
static int parse_expression(Parser *parser);
static int parse_add_expr(Parser *parser);
static int parse_term(Parser *parser);
static int parse_mul_expr(Parser *parser);
static int parse_factor(Parser *parser);

/* Program → Statements */
static int parse_program(Parser *parser) {
    printf("Parsing Program\n");
    return parse_statements(parser);
}

/* Statements → Statement Statements | ε */
static int parse_statements(Parser *parser) {
    printf("Parsing Statements\n");
    
    /* Check if we can parse a statement */
    if (parser->current_token.type == TOK_KEYWORD_INT ||
        parser->current_token.type == TOK_KEYWORD_CHAR ||
        parser->current_token.type == TOK_IDENTIFIER ||
        parser->current_token.type == TOK_KEYWORD_IF ||
        parser->current_token.type == TOK_KEYWORD_WHILE ||
        parser->current_token.type == TOK_KEYWORD_FOR) {
        
        if (!parse_statement(parser)) return 0;
        if (!parse_statements(parser)) return 0;
    }
    /* ε production - just return success */
    return 1;
}

/* Statement → Declaration | Assignment | IfStatement | WhileStatement | ForStatement */
static int parse_statement(Parser *parser) {
    printf("Parsing Statement\n");
    
    if (parser->current_token.type == TOK_KEYWORD_INT ||
        parser->current_token.type == TOK_KEYWORD_CHAR) {
        return parse_declaration(parser);
    } else if (parser->current_token.type == TOK_IDENTIFIER) {
        return parse_assignment(parser);
    } else if (parser->current_token.type == TOK_KEYWORD_IF) {
        return parse_if_statement(parser);
    } else if (parser->current_token.type == TOK_KEYWORD_WHILE) {
        return parse_while_statement(parser);
    } else if (parser->current_token.type == TOK_KEYWORD_FOR) {
        return parse_for_statement(parser);
    }
    
    parser_print_error("Invalid statement");
    return 0;
}

/* Declaration → int IDENTIFIER | char IDENTIFIER */
static int parse_declaration(Parser *parser) {
    printf("Parsing Declaration\n");
    
    if (parser->current_token.type == TOK_KEYWORD_INT) {
        printf("  TYPE: int\n");
        lexer_read_token(parser);
    } else if (parser->current_token.type == TOK_KEYWORD_CHAR) {
        printf("  TYPE: char\n");
        lexer_read_token(parser);
    } else {
        parser_print_error("Expected 'int' or 'char' in declaration");
        return 0;
    }
    
    if (parser->current_token.type != TOK_IDENTIFIER) {
        parser_print_error("Expected identifier in declaration");
        return 0;
    }
    
    printf("  IDENTIFIER: %s\n", parser->current_token.value);
    lexer_read_token(parser);
    return 1;
}

/* Assignment → IDENTIFIER = Expression */
static int parse_assignment(Parser *parser) {
    printf("Parsing Assignment\n");
    
    if (parser->current_token.type != TOK_IDENTIFIER) {
        parser_print_error("Expected identifier in assignment");
        return 0;
    }
    
    printf("  IDENTIFIER: %s\n", parser->current_token.value);
    lexer_read_token(parser);
    
    if (parser->current_token.type != TOK_ASSIGN) {
        parser_print_error("Expected '=' in assignment");
        return 0;
    }
    
    printf("  OPERATOR: =\n");
    lexer_read_token(parser);
    
    return parse_expression(parser);
}

/* IfStatement → if Expression Statement */
static int parse_if_statement(Parser *parser) {
    printf("Parsing IfStatement\n");
    
    if (parser->current_token.type != TOK_KEYWORD_IF) {
        parser_print_error("Expected 'if'");
        return 0;
    }
    
    lexer_read_token(parser);
    
    if (!parse_expression(parser)) return 0;
    if (!parse_statement(parser)) return 0;
    
    return 1;
}

/* WhileStatement → while Expression Statement */
static int parse_while_statement(Parser *parser) {
    printf("Parsing WhileStatement\n");
    
    if (parser->current_token.type != TOK_KEYWORD_WHILE) {
        parser_print_error("Expected 'while'");
        return 0;
    }
    
    lexer_read_token(parser);
    
    if (!parse_expression(parser)) return 0;
    if (!parse_statement(parser)) return 0;
    
    return 1;
}

/* ForStatement → for Assignment Statement */
static int parse_for_statement(Parser *parser) {
    printf("Parsing ForStatement\n");
    
    if (parser->current_token.type != TOK_KEYWORD_FOR) {
        parser_print_error("Expected 'for'");
        return 0;
    }
    
    lexer_read_token(parser);
    
    if (!parse_assignment(parser)) return 0;
    if (!parse_statement(parser)) return 0;
    
    return 1;
}

/* Expression → Term AddExpr */
static int parse_expression(Parser *parser) {
    printf("Parsing Expression\n");
    
    if (!parse_term(parser)) return 0;
    if (!parse_add_expr(parser)) return 0;
    
    return 1;
}

/* AddExpr → + Term AddExpr | ε */
static int parse_add_expr(Parser *parser) {
    printf("Parsing AddExpr\n");
    
    if (parser->current_token.type == TOK_PLUS) {
        printf("  OPERATOR: +\n");
        lexer_read_token(parser);
        if (!parse_term(parser)) return 0;
        if (!parse_add_expr(parser)) return 0;
    }
    /* ε production */
    return 1;
}

/* Term → Factor MulExpr */
static int parse_term(Parser *parser) {
    printf("Parsing Term\n");
    
    if (!parse_factor(parser)) return 0;
    if (!parse_mul_expr(parser)) return 0;
    
    return 1;
}

/* MulExpr → * Factor MulExpr | % Factor MulExpr | ε */
static int parse_mul_expr(Parser *parser) {
    printf("Parsing MulExpr\n");
    
    if (parser->current_token.type == TOK_MUL) {
        printf("  OPERATOR: *\n");
        lexer_read_token(parser);
        if (!parse_factor(parser)) return 0;
        if (!parse_mul_expr(parser)) return 0;
    } else if (parser->current_token.type == TOK_MOD) {
        printf("  OPERATOR: %%\n");
        lexer_read_token(parser);
        if (!parse_factor(parser)) return 0;
        if (!parse_mul_expr(parser)) return 0;
    }
    /* ε production */
    return 1;
}

/* Factor → IDENTIFIER | NUMBER */
static int parse_factor(Parser *parser) {
    printf("Parsing Factor\n");
    
    if (parser->current_token.type == TOK_IDENTIFIER) {
        printf("  IDENTIFIER: %s\n", parser->current_token.value);
        lexer_read_token(parser);
        return 1;
    } else if (parser->current_token.type == TOK_NUMBER) {
        printf("  NUMBER: %s\n", parser->current_token.value);
        lexer_read_token(parser);
        return 1;
    }
    
    parser_print_error("Expected identifier or number in factor");
    return 0;
}

/* Main parsing function */
int parser_parse(Parser *parser) {
    printf("=== LL(1) Parser Starting ===\n\n");
    
    if (parse_program(parser)) {
        if (parser->current_token.type == TOK_EOF) {
            printf("\n=== Parsing Successful ===\n");
            return 1;
        } else {
            printf("\nERROR: Expected EOF, got %s\n", parser->current_token.value);
            return 0;
        }
    }
    
    printf("\n=== Parsing Failed ===\n");
    return 0;
}
