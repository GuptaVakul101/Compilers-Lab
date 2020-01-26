#include "lex.h"
#include <stdio.h>
#include <ctype.h>

int yylineno = 0;
char* yytext = "";
int yyleng   = 0;

int lex(void){
    char *current;
    static char input_buffer[2020];
    current = yytext + yyleng;
    while(4){
        while(!*current ){
            current = input_buffer;
            if(gets(input_buffer)==0){
                *current = '\0' ;
                return EOI;
            }
            for(;isspace(*current);current++){
            }
            yylineno++;
        }
        for(; *current; ++current){
            /* Get the next token */
            yytext = current;
            yyleng = 1;
            char help=(*current);
            if(help==';'){
                return SEMI;
            }
            if(help=='+'){
                return PLUS;
            }
            if(help=='-'){
                return MINUS;
            }
            if(help=='*'){
                return TIMES;
            }
            if(help=='/'){
                return DIV;
            }
            if(help=='('){
                return LP;
            }
            if(help==')'){
                return RP;
            }
            if(help=='>'){
                return GT;
            }
            if(help=='<'){
                return LT;
            }
            if(help=='='){
                return EQUAL;
            }
            if(help==':'){
                return COL;
            }
            if(!isalnum(*current))
            fprintf(stderr, "Not alphanumeric <%c>\n", *current);
            else{
                for(;isalnum(*current);current++){
                }
                yyleng = current - yytext;
                char *check=(char *)(malloc((yyleng+10)*sizeof(char)));
                int i=0;
                for(i=0;i<yyleng;i++){
                    check[i]=yytext[i];
                }
                check[yyleng]='\0';
                if(strcmp(check,"if")==0){
                    return IF;
                }
                if(strcmp(check,"do")==0){
                    return DO;
                }
                if(strcmp(check,"end")==0){
                    return END;
                }
                if(strcmp(check,"then")==0){
                    return THEN;
                }
                if(strcmp(check,"begin")==0){
                    return BEGIN;
                }
                if(strcmp(check,"while")==0){
                    return WHILE;
                }
                if(yyleng>0 && isalpha(yytext[0]))
                {
                    for(;(isalnum(*current) || isspace(*current));current++){
                    }
                    char help2=(*current);
                    if(help2==':'){
                        return ID;
                    }
                }
                return NUM_OR_ID;
            }
        }
    }
}
static int Lookahead = -1;
int match(int token){
    if(Lookahead == -1)
        Lookahead = lex();
    int ans=-1;
    if(token==Lookahead){
        ans=1;
    }else{
        ans=0;
    }
    return ans;
}

void advance(void){
    Lookahead = lex();
}
