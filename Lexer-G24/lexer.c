/* 

This contains the implementation of the lexer for the sub-language specified by group 24
The members include : 
     ODHIAMBO JOHN OTIENO - SCS3/147357/2023
     MICHAEL MUTETI - SCS3/147532/2023
     ABBA BIKO WERE - SCS3/146750/2023
     MARK WAFULA - SCS3/146670/2023

*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Keywords from the mini grammar */
char *keywords[] = {"if","while","for","int","char"};
int keywordCount = 5;

/* Check if lexeme is a keyword and return the length, or -1 if not */
int isKeyword(char *word){
    for(int i=0;i<keywordCount;i++){
        if(strcmp(word,keywords[i])==0)
            return strlen(keywords[i]);
    }
    return -1;
}

/* Find the longest keyword prefix in the lexeme */
int getLongestKeywordPrefix(char *lexeme){
    char temp[100];
    
    for(int len = strlen(lexeme); len >= 1; len--){
        strncpy(temp, lexeme, len);
        temp[len] = '\0';
        
        for(int i = 0; i < keywordCount; i++){
            if(strcmp(temp, keywords[i]) == 0){
                return len;
            }
        }
    }
    return -1;
}

int main(){

    FILE *fp;
    char ch;
    char lexeme[100];
    int i;
    int line_number = 1;  /* Track line numbers */
    int error_count = 0;  /* Track errors for exit code */
    
    /* Buffer for unprocessed characters */
    char unprocessed[100];
    int unprocessed_count = 0;
    int unprocessed_idx = 0;

    /* DFA States */
    enum {S0, S1, S2, S3, S4};
    int state = S0;

    fp = fopen("program.txt","r");

    if(fp == NULL){
        printf("Error opening file\n");
        return 1;
    }

    while(1){
        /* Read from unprocessed buffer first, then from file */
        if(unprocessed_idx < unprocessed_count) {
            ch = unprocessed[unprocessed_idx++];
        } else {
            ch = fgetc(fp);
            if(ch == EOF) break;
        }

        switch(state){

            /* ---------- START STATE ---------- */
            case S0:

                if(isalpha(ch)){          /* letter -> identifier */
                    state = S1;
                    i = 0;
                    lexeme[i++] = ch;
                }

                else if(isdigit(ch)){     /* digit -> number */
                    state = S2;
                    i = 0;
                    lexeme[i++] = ch;
                }

                else if(ch=='=' || ch=='+' || ch=='*' || ch=='-' || ch=='%' || ch=='<' || ch=='>')
                {  /* operator */
                    state = S3;
                    printf("LINE %d: OPERATOR: %c\n", line_number, ch);
                    state = S0;
                }

                else if(ch == '\n'){
                    line_number++;  /* Increment line number on newline */
                    continue;
                }

                else if(isspace(ch)){
                    continue;
                }

                else{
                    state = S4;
                    printf("LINE %d: ERROR: Invalid character '%c'\n", line_number, ch);
                    error_count++;
                    state = S0;
                }

            break;

            /* ---------- IDENTIFIER STATE ---------- */
            case S1:

                if(isalnum(ch)){
                    lexeme[i++] = ch;
                }
                else{
                    lexeme[i] = '\0';
                    
                    /* Check for keyword prefix */
                    int keywordLen = getLongestKeywordPrefix(lexeme);
                    if(keywordLen > 0){
                        char keyword[100];
                        strncpy(keyword, lexeme, keywordLen);
                        keyword[keywordLen] = '\0';
                        printf("LINE %d: KEYWORD: %s\n", line_number, keyword);
                        
                        /* Put remainder in unprocessed buffer */
                        unprocessed_count = i - keywordLen + 1;
                        unprocessed_idx = 0;
                        for(int j = 0; j < i - keywordLen; j++){
                            unprocessed[j] = lexeme[keywordLen + j];
                        }
                        unprocessed[i - keywordLen] = ch;
                    }
                    else{
                        printf("LINE %d: IDENTIFIER: %s\n", line_number, lexeme);
                        
                        /* Put current character in unprocessed buffer */
                        unprocessed_count = 1;
                        unprocessed_idx = 0;
                        unprocessed[0] = ch;
                    }

                    state = S0;
                }

            break;

            /* ---------- NUMBER STATE ---------- */
            case S2:

                if(isdigit(ch)){
                    lexeme[i++] = ch;
                }
                else{
                    lexeme[i] = '\0';
                    printf("LINE %d: NUMBER: %s\n", line_number, lexeme);

                    /* Put current character in unprocessed buffer */
                    unprocessed_count = 1;
                    unprocessed_idx = 0;
                    unprocessed[0] = ch;
                    
                    state = S0;
                }

            break;
        }
    }

    /* Handle EOF cases */
    if(state == S1){
        lexeme[i] = '\0';
        
        int keywordLen = getLongestKeywordPrefix(lexeme);
        if(keywordLen > 0){
            char keyword[100];
            strncpy(keyword, lexeme, keywordLen);
            keyword[keywordLen] = '\0';
            printf("LINE %d: KEYWORD: %s\n", line_number, keyword);
            
            /* Check if there's a remainder */
            if(keywordLen < i){
                char remainder[100];
                strcpy(remainder, lexeme + keywordLen);
                printf("LINE %d: IDENTIFIER: %s\n", line_number, remainder);
            }
        }
        else{
            printf("LINE %d: IDENTIFIER: %s\n", line_number, lexeme);
        }
    }

    if(state == S2){
        lexeme[i] = '\0';
        printf("LINE %d: NUMBER: %s\n", line_number, lexeme);
    }

    fclose(fp);
    
    /* Return error code if any errors found */
    if(error_count > 0){
        fprintf(stderr, "\nLexer completed with %d error(s)\n", error_count);
        return 1;
    }
    
    return 0;
}
