/*
LL(1) Parser for Mini-Grammar with Parse Tree Generation
Uses recursive descent parsing with tokens from lexer

Group 24 Members:
     ODHIAMBO JOHN OTIENO - SCS3/147357/2023
     MICHAEL MUTETI - SCS3/147532/2024
     ABBA BIKO WERE - SCS3/146750/2023
     MARK WAFULA - SCS3/146670/2023
*/

#include "parser.h"
#include <ctype.h>

/* Forward declarations of parsing functions */
static ParseTreeNode* parse_program(Parser *parser);
static ParseTreeNode* parse_statements(Parser *parser);
static ParseTreeNode* parse_statement(Parser *parser);
static ParseTreeNode* parse_declaration(Parser *parser);
static ParseTreeNode* parse_assignment(Parser *parser);
static ParseTreeNode* parse_if_statement(Parser *parser);
static ParseTreeNode* parse_while_statement(Parser *parser);
static ParseTreeNode* parse_for_statement(Parser *parser);
static ParseTreeNode* parse_expression(Parser *parser);
static ParseTreeNode* parse_add_expr(Parser *parser);
static ParseTreeNode* parse_term(Parser *parser);
static ParseTreeNode* parse_mul_expr(Parser *parser);
static ParseTreeNode* parse_factor(Parser *parser);

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
static ParseTreeNode* parse_program(Parser *parser) {
    ParseTreeNode *node = ast_create_node(NODE_PROGRAM, NULL);
    ParseTreeNode *child = parse_statements(parser);
    
    if (child) {
        ast_add_child(node, child);
    }
    
    return node;
}

/* Statements → Statement Statements | ε */
static ParseTreeNode* parse_statements(Parser *parser) {
    ParseTreeNode *node = ast_create_node(NODE_STATEMENTS, NULL);
    
    /* Check if we can parse a statement */
    if (parser->current_token.type == TOK_KEYWORD_INT ||
        parser->current_token.type == TOK_KEYWORD_CHAR ||
        parser->current_token.type == TOK_IDENTIFIER ||
        parser->current_token.type == TOK_KEYWORD_IF ||
        parser->current_token.type == TOK_KEYWORD_WHILE ||
        parser->current_token.type == TOK_KEYWORD_FOR) {
        
        ParseTreeNode *stmt = parse_statement(parser);
        if (stmt) {
            ast_add_child(node, stmt);
            ParseTreeNode *rest = parse_statements(parser);
            if (rest && rest->child_count > 0) {
                ast_add_child(node, rest);
            } else {
                ast_free_tree(rest);
            }
        }
    } else {
        /* ε production */
        ast_add_child(node, ast_create_node(NODE_EPSILON, NULL));
    }
    
    return node;
}

/* Statement → Declaration | Assignment | IfStatement | WhileStatement | ForStatement */
static ParseTreeNode* parse_statement(Parser *parser) {
    ParseTreeNode *node = ast_create_node(NODE_STATEMENT, NULL);
    ParseTreeNode *child = NULL;
    
    if (parser->current_token.type == TOK_KEYWORD_INT ||
        parser->current_token.type == TOK_KEYWORD_CHAR) {
        child = parse_declaration(parser);
    } else if (parser->current_token.type == TOK_IDENTIFIER) {
        child = parse_assignment(parser);
    } else if (parser->current_token.type == TOK_KEYWORD_IF) {
        child = parse_if_statement(parser);
    } else if (parser->current_token.type == TOK_KEYWORD_WHILE) {
        child = parse_while_statement(parser);
    } else if (parser->current_token.type == TOK_KEYWORD_FOR) {
        child = parse_for_statement(parser);
    } else {
        parser_print_error("Invalid statement");
        ast_free_tree(node);
        return NULL;
    }
    
    if (child) {
        ast_add_child(node, child);
    }
    
    return node;
}

/* Declaration → int IDENTIFIER | char IDENTIFIER */
static ParseTreeNode* parse_declaration(Parser *parser) {
    ParseTreeNode *node = ast_create_node(NODE_DECLARATION, NULL);
    
    if (parser->current_token.type == TOK_KEYWORD_INT) {
        ast_add_child(node, ast_create_node(NODE_KEYWORD, "int"));
        lexer_read_token(parser);
    } else if (parser->current_token.type == TOK_KEYWORD_CHAR) {
        ast_add_child(node, ast_create_node(NODE_KEYWORD, "char"));
        lexer_read_token(parser);
    } else {
        parser_print_error("Expected 'int' or 'char' in declaration");
        ast_free_tree(node);
        return NULL;
    }
    
    if (parser->current_token.type != TOK_IDENTIFIER) {
        parser_print_error("Expected identifier in declaration");
        ast_free_tree(node);
        return NULL;
    }
    
    ast_add_child(node, ast_create_node(NODE_IDENTIFIER, parser->current_token.value));
    lexer_read_token(parser);
    
    return node;
}

/* Assignment → IDENTIFIER = Expression */
static ParseTreeNode* parse_assignment(Parser *parser) {
    ParseTreeNode *node = ast_create_node(NODE_ASSIGNMENT, NULL);
    
    if (parser->current_token.type != TOK_IDENTIFIER) {
        parser_print_error("Expected identifier in assignment");
        ast_free_tree(node);
        return NULL;
    }
    
    ast_add_child(node, ast_create_node(NODE_IDENTIFIER, parser->current_token.value));
    lexer_read_token(parser);
    
    if (parser->current_token.type != TOK_ASSIGN) {
        parser_print_error("Expected '=' in assignment");
        ast_free_tree(node);
        return NULL;
    }
    
    ast_add_child(node, ast_create_node(NODE_OPERATOR, "="));
    lexer_read_token(parser);
    
    ParseTreeNode *expr = parse_expression(parser);
    if (expr) {
        ast_add_child(node, expr);
    } else {
        ast_free_tree(node);
        return NULL;
    }
    
    return node;
}

/* IfStatement → if Expression Statement */
static ParseTreeNode* parse_if_statement(Parser *parser) {
    ParseTreeNode *node = ast_create_node(NODE_IF_STATEMENT, NULL);
    
    if (parser->current_token.type != TOK_KEYWORD_IF) {
        parser_print_error("Expected 'if'");
        ast_free_tree(node);
        return NULL;
    }
    
    ast_add_child(node, ast_create_node(NODE_KEYWORD, "if"));
    lexer_read_token(parser);
    
    ParseTreeNode *expr = parse_expression(parser);
    if (expr) {
        ast_add_child(node, expr);
    } else {
        ast_free_tree(node);
        return NULL;
    }
    
    ParseTreeNode *stmt = parse_statement(parser);
    if (stmt) {
        ast_add_child(node, stmt);
    } else {
        ast_free_tree(node);
        return NULL;
    }
    
    return node;
}

/* WhileStatement → while Expression Statement */
static ParseTreeNode* parse_while_statement(Parser *parser) {
    ParseTreeNode *node = ast_create_node(NODE_WHILE_STATEMENT, NULL);
    
    if (parser->current_token.type != TOK_KEYWORD_WHILE) {
        parser_print_error("Expected 'while'");
        ast_free_tree(node);
        return NULL;
    }
    
    ast_add_child(node, ast_create_node(NODE_KEYWORD, "while"));
    lexer_read_token(parser);
    
    ParseTreeNode *expr = parse_expression(parser);
    if (expr) {
        ast_add_child(node, expr);
    } else {
        ast_free_tree(node);
        return NULL;
    }
    
    ParseTreeNode *stmt = parse_statement(parser);
    if (stmt) {
        ast_add_child(node, stmt);
    } else {
        ast_free_tree(node);
        return NULL;
    }
    
    return node;
}

/* ForStatement → for Assignment Statement */
static ParseTreeNode* parse_for_statement(Parser *parser) {
    ParseTreeNode *node = ast_create_node(NODE_FOR_STATEMENT, NULL);
    
    if (parser->current_token.type != TOK_KEYWORD_FOR) {
        parser_print_error("Expected 'for'");
        ast_free_tree(node);
        return NULL;
    }
    
    ast_add_child(node, ast_create_node(NODE_KEYWORD, "for"));
    lexer_read_token(parser);
    
    ParseTreeNode *assign = parse_assignment(parser);
    if (assign) {
        ast_add_child(node, assign);
    } else {
        ast_free_tree(node);
        return NULL;
    }
    
    ParseTreeNode *stmt = parse_statement(parser);
    if (stmt) {
        ast_add_child(node, stmt);
    } else {
        ast_free_tree(node);
        return NULL;
    }
    
    return node;
}

/* Expression → Term AddExpr */
static ParseTreeNode* parse_expression(Parser *parser) {
    ParseTreeNode *node = ast_create_node(NODE_EXPRESSION, NULL);
    
    ParseTreeNode *term = parse_term(parser);
    if (term) {
        ast_add_child(node, term);
    } else {
        ast_free_tree(node);
        return NULL;
    }
    
    ParseTreeNode *add_expr = parse_add_expr(parser);
    if (add_expr) {
        ast_add_child(node, add_expr);
    } else {
        ast_free_tree(node);
        return NULL;
    }
    
    return node;
}

/* AddExpr → + Term AddExpr | ε */
static ParseTreeNode* parse_add_expr(Parser *parser) {
    ParseTreeNode *node = ast_create_node(NODE_TERM, NULL);
    
    if (parser->current_token.type == TOK_PLUS) {
        ast_add_child(node, ast_create_node(NODE_OPERATOR, "+"));
        lexer_read_token(parser);
        
        ParseTreeNode *term = parse_term(parser);
        if (term) {
            ast_add_child(node, term);
        } else {
            ast_free_tree(node);
            return NULL;
        }
        
        ParseTreeNode *rest = parse_add_expr(parser);
        if (rest && rest->child_count > 0) {
            ast_add_child(node, rest);
        } else {
            ast_free_tree(rest);
        }
    } else {
        /* ε production */
        ast_add_child(node, ast_create_node(NODE_EPSILON, NULL));
    }
    
    return node;
}

/* Term → Factor MulExpr */
static ParseTreeNode* parse_term(Parser *parser) {
    ParseTreeNode *node = ast_create_node(NODE_TERM, NULL);
    
    ParseTreeNode *factor = parse_factor(parser);
    if (factor) {
        ast_add_child(node, factor);
    } else {
        ast_free_tree(node);
        return NULL;
    }
    
    ParseTreeNode *mul_expr = parse_mul_expr(parser);
    if (mul_expr) {
        ast_add_child(node, mul_expr);
    } else {
        ast_free_tree(node);
        return NULL;
    }
    
    return node;
}

/* MulExpr → * Factor MulExpr | % Factor MulExpr | ε */
static ParseTreeNode* parse_mul_expr(Parser *parser) {
    ParseTreeNode *node = ast_create_node(NODE_TERM, NULL);
    
    if (parser->current_token.type == TOK_MUL) {
        ast_add_child(node, ast_create_node(NODE_OPERATOR, "*"));
        lexer_read_token(parser);
        
        ParseTreeNode *factor = parse_factor(parser);
        if (factor) {
            ast_add_child(node, factor);
        } else {
            ast_free_tree(node);
            return NULL;
        }
        
        ParseTreeNode *rest = parse_mul_expr(parser);
        if (rest && rest->child_count > 0) {
            ast_add_child(node, rest);
        } else {
            ast_free_tree(rest);
        }
    } else if (parser->current_token.type == TOK_MOD) {
        ast_add_child(node, ast_create_node(NODE_OPERATOR, "%"));
        lexer_read_token(parser);
        
        ParseTreeNode *factor = parse_factor(parser);
        if (factor) {
            ast_add_child(node, factor);
        } else {
            ast_free_tree(node);
            return NULL;
        }
        
        ParseTreeNode *rest = parse_mul_expr(parser);
        if (rest && rest->child_count > 0) {
            ast_add_child(node, rest);
        } else {
            ast_free_tree(rest);
        }
    } else {
        /* ε production */
        ast_add_child(node, ast_create_node(NODE_EPSILON, NULL));
    }
    
    return node;
}

/* Factor → IDENTIFIER | NUMBER */
static ParseTreeNode* parse_factor(Parser *parser) {
    ParseTreeNode *node = ast_create_node(NODE_FACTOR, NULL);
    
    if (parser->current_token.type == TOK_IDENTIFIER) {
        ast_add_child(node, ast_create_node(NODE_IDENTIFIER, parser->current_token.value));
        lexer_read_token(parser);
        return node;
    } else if (parser->current_token.type == TOK_NUMBER) {
        ast_add_child(node, ast_create_node(NODE_NUMBER, parser->current_token.value));
        lexer_read_token(parser);
        return node;
    }
    
    parser_print_error("Expected identifier or number in factor");
    ast_free_tree(node);
    return NULL;
}

/* Main parsing function - returns parse tree root */
ParseTreeNode* parser_parse(Parser *parser) {
    printf("=== LL(1) Parser Starting ===\n\n");
    
    ParseTreeNode *root = parse_program(parser);
    
    if (root && parser->current_token.type == TOK_EOF) {
        printf("\n=== Parsing Successful ===\n");
        return root;
    }
    
    if (root) {
        printf("\nERROR: Expected EOF, got %s\n", parser->current_token.value);
        ast_free_tree(root);
    }
    
    printf("\n=== Parsing Failed ===\n");
    return NULL;
}
