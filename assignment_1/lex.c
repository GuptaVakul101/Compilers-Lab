#include "lex.h"
#include <stdio.h>
#include <ctype.h>

int yylineno = 0;
char* yytext = "";
int yyleng   = 0;

int lex(void)
{
    char *pres_p;
    static char arr[2020];
    pres_p = yytext + yyleng;

    for(int i=1;i>0;)
    {
        while(!*pres_p )
        {
            pres_p = arr;

            if(gets(arr) == 0)
            {
                *pres_p = '\0' ;
                return EOI;
            }
            while(*pres_p == ' ' || *pres_p == '\n' || *pres_p == '\t' || *pres_p == '\v' || *pres_p == '\f' || *pres_p == '\r'){
                pres_p++;
            }
            yylineno++;
        }
        while(1)
        {
        	if(*pres_p == '\0'){
                break;
            }

            yytext = pres_p;
            yyleng = 1;

            char check_c=(*pres_p);
            int answer = -1;


           /*switch( *pres_p ){
	           case ';':
	            return SEMI;
	           case '+':
	            return PLUS;
	           case '-':
	            return MINUS;
	           case '*':
	            return TIMES;
	           case '/':
	            return DIV;
	           case '(':
	            return LP;
	           case ')':
	            return RP;
	           case '>':
	            return GT;
	           case '<':
	            return LT;
	           case '=':
	            return EQUAL;
	           case ':':
	            return COL;
	           case '\n':
	           case '\t':
	           case ' ' :
	            break;
	            default:
	            break;
        	}*/

        	if(check_c == ';'){
                answer = SEMI;
            }
            else if(check_c == '+'){
                answer = PLUS;
            }
            else if(check_c == '-'){
                answer = MINUS;
            }
            else if(check_c == '*'){
                answer = TIMES;
            }
            else if(check_c == '/'){
                answer = DIV;
            }
            else if(check_c == '('){
                answer = LP;
            }
            else if(check_c == ')'){
                answer = RP;
            }
            else if(check_c == '>'){
                answer = GT;
            }
            else if(check_c == '<'){
                answer = LT;
            }
            else if(check_c=='='){
                answer = EQUAL;
            }
            else if(check_c==':'){
                answer = COL;
            }
        	
        	if(answer != -1)
        	{
        		return answer;
        	}
        	if(!(check_c == ' ' || check_c == '\n' || check_c == '\t'))
        	{
	            if(!isalnum(check_c))
	            {
	               fprintf(stderr, "Not alphanumeric <%c>\n", check_c);
	            }
	            else
	            {
	                for(;isalnum(*pres_p);pres_p++){
	                }
	                yyleng = pres_p - yytext;
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
	                    for(;(isalnum(*pres_p) || isspace(*pres_p));pres_p++){
	                    }
	                    char help2=(*pres_p);
	                    if(help2==':'){
	                        return ID;
	                    }
	                }
	                return NUM_OR_ID;
	            }
	        }
           
        pres_p++;
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
