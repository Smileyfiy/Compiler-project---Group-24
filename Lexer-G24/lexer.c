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

/* Check if lexeme is a keyword */
int isKeyword(char *word){
    for(int i=0;i<keywordCount;i++){
        if(strcmp(word,keywords[i])==0)
            return 1;
    }
    return 0;
}

int main(){

    FILE *fp;
    char ch;
    char lexeme[100];
    int i;

    /* DFA States */
    enum {S0, S1, S2, S3, S4};
    int state = S0;

    fp = fopen("program.txt","r");

    if(fp == NULL){
        printf("Error opening file\n");
        return 1;
    }

    while((ch = fgetc(fp)) != EOF){

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

                else if(ch=='=' || ch=='+' || ch=='*' || ch=='%'){
                    state = S3;
                    printf("OPERATOR: %c\n", ch);
                    state = S0;
                }

                else if(isspace(ch)){
                    continue;
                }

                else{
                    state = S4;
                    printf("ERROR: %c\n", ch);
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

                    if(isKeyword(lexeme))
                        printf("KEYWORD: %s\n", lexeme);
                    else
                        printf("IDENTIFIER: %s\n", lexeme);

                    state = S0;
                    ungetc(ch, fp);
                }

            break;

            /* ---------- NUMBER STATE ---------- */
            case S2:

                if(isdigit(ch)){
                    lexeme[i++] = ch;
                }
                else{
                    lexeme[i] = '\0';
                    printf("NUMBER: %s\n", lexeme);

                    state = S0;
                    ungetc(ch, fp);
                }

            break;
        }
    }

    /* Handle EOF cases */
    if(state == S1){
        lexeme[i] = '\0';
        if(isKeyword(lexeme))
            printf("KEYWORD: %s\n", lexeme);
        else
            printf("IDENTIFIER: %s\n", lexeme);
    }

    if(state == S2){
        lexeme[i] = '\0';
        printf("NUMBER: %s\n", lexeme);
    }

    fclose(fp);
    return 0;
}
