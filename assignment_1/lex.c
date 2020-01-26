#include "lex.h"
#include <stdio.h>
#include <ctype.h>

int line_number = 0;
char* cur_para = "";
int cur_size   = 0;

int lex(void)
{
    char *pres_p;
    static char arr[2020];
    pres_p = cur_para + cur_size;

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
            line_number++;
        }
        while(1)
        {
        	if(*pres_p == '\0'){
                break;
            }

            cur_para = pres_p;
            cur_size = 1;

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
	                cur_size = pres_p - cur_para;
	                char *check=(char *)(malloc((cur_size+10)*sizeof(char)));

	                int i=0;
	                for(i=0;i<cur_size;i++)
	                {
	                    check[i]=cur_para[i];
	                }

	                check[cur_size]='\0';

	                if(strcmp(check,"if") == 0){
	                    return IF;
	                }
	                else if(strcmp(check,"do") == 0){
	                    return DO;
	                }
	                else if(strcmp(check,"end") == 0){
	                    return END;
	                }
	                else if(strcmp(check,"then") == 0){
	                    return THEN;
	                }
	                else if(strcmp(check,"begin") == 0){
	                    return BEGIN;
	                }
	                else if(strcmp(check,"while") == 0){
	                    return WHILE;
	                }

	                if(cur_size>0)
	                {
	                	if(isalpha(cur_para[0]))
	                	{
	                		for(;(isalnum(*pres_p) || isspace(*pres_p));pres_p++){
		                    }
		                    char cur_char=(*pres_p);

		                    if(cur_char==':')
		                    {
		                        return ID;
		                    }
	                	}
	                }
	                return NUM_OR_ID;
	            }
	        }

        pres_p++;

        }
    }
}

static int next_tok = -1;
int common_check(int symb)
{
    if(next_tok == -1)
    {
        next_tok = lex();
    }

    int ans = 1;
    if(symb == next_tok)
    {
        ans=1;
    }
    else
    {
        ans=0;
    }
    return ans;
}

void look_forward(void)
{
    next_tok = lex();
}
