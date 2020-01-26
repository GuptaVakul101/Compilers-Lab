#include <stdio.h>
#include "lex.h"
#include "lex.c"
#include "name.c"
#include <stdbool.h>

#include <stdarg.h>

#define MAXFIRST 16
#define SYNCH    SEMI

char *factor     ( void );
char *term       ( void );
char *expression ( void );

extern char *newname( void       );
extern void releaseMem( char *name );
char * expression1();
int correctFutureBuffer( int first_arg  , ...);

void statements(){
    while(!match(EOI))
        statement();
}

void statement()
{
    /*  statements -> expression SEMI  |  expression SEMI statements  */
        char *strTemp;
        if(match(ID))
        {
            char array[100];
            for(int i=0;i<yyleng;i++)
                array[i]=*(yytext+i);
            array[yyleng]='\0';



            advance();  
            if(!correctFutureBuffer( COL, 0 ) ){
                printf("%d: Inserting missing colon\n", yylineno );
                goto confirm_semiColon;
            }
            else{


                advance();
                if( !correctFutureBuffer( EQUAL, 0 ) ){
                    printf("%d: Inserting missing equal\n", yylineno );
                    goto confirm_semiColon;
                }
                else{
                    advance();
                    strTemp = expression1();
                    printf("_%s <- %s\n",array,strTemp);
                }
            }
        }
        else if(match(IF)){
            advance();
            printf("if (\n");
            strTemp = expression1();
            if( !correctFutureBuffer( THEN, 0 ) )
            {
                releaseMem(strTemp);
                printf("%d: Inserting missing then\n", yylineno );
                goto confirm_semiColon;
            }
            else{
                printf("%s)\nthen {\n", strTemp);
                advance();
                releaseMem(strTemp);
                statement();
                printf("}\n");
                return;
            }
        }
        else if(match(WHILE))
        {
            advance();
            printf("while (\n");
            strTemp = expression1();
            if( !correctFutureBuffer( DO, 0 ) )
            {
                releaseMem(strTemp);
                printf("%d: Inserting missing do\n", yylineno );
                goto confirm_semiColon;
            }
            else
            {
                printf("%s)\n do {\n", strTemp);
                advance();
                releaseMem(strTemp);
                statement();
                printf("}\n");
                return;
            }
        }
        else if(match(BEGIN))
        {
            printf("BEGIN{\n");

            advance();
            stmt_list();
            if(!correctFutureBuffer(END,0))
            {
                printf("%d: Inserting missing END\n", yylineno );
                goto confirm_semiColon;
            } 
            else 
            {
                printf("} END\n");
                advance();
            }
            return;
        }
        else{
            strTemp = expression1();
        }

        if(strTemp!=NULL){
            releaseMem( strTemp );
        } 
        else{
            exit(1);
        }
        // strTemp = expression();
        confirm_semiColon:
            if( match( SEMI ) )
                advance();
            else
                printf("%d: Inserting missing semicolon\n", yylineno );

        // releaseMem( strTemp );
}

void stmt_list()
{
    while(!match(END)&&!match(EOI))
        statement();
    if(match(EOI)){
        fprintf( stderr, "%d: End of file reached no END found\n", yylineno );
    }
}

char *expression1(){
    char *strTemp3;
    char * strTemp=expression();
    if(match(GT))
    {
        releaseMem(strTemp);
        strTemp3=newname();
        strTemp=newname();
        printf("%s <- %s\n", strTemp, strTemp3);

        advance();
        char *strTemp2=expression();
        printf("%s <-  %s > %s\n",strTemp3,strTemp,strTemp2);
        
        releaseMem(strTemp2);


        releaseMem(strTemp);
        return strTemp3;
    }
    else if(match(LT))
    {
        releaseMem(strTemp);
        strTemp3=newname();
        strTemp=newname();
        printf("%s <- %s\n", strTemp, strTemp3);

        advance();
        char *strTemp2=expression();
        printf("%s <-  %s < %s\n",strTemp3,strTemp,strTemp2);
        releaseMem(strTemp);
        releaseMem(strTemp2);
        return strTemp3;
    }
    else if(match(EQUAL))
    {
        releaseMem(strTemp);
        strTemp3=newname();
        strTemp=newname();
        printf("%s <- %s\n", strTemp, strTemp3);

        advance();
        char *strTemp2=expression();
        printf("%s <-  %s == %s\n",strTemp3,strTemp,strTemp2);
        releaseMem(strTemp);
        releaseMem(strTemp2);
        return strTemp3;
    }
    return strTemp;
}
char    *expression()
{
    char  *strTemp, *strTemp2;

    strTemp = term();
    bool flag=true;
    while(flag)
    {
        if(match( PLUS )){
            advance();
            strTemp2 = term();
            printf("%s += %s\n", strTemp, strTemp2 );
            releaseMem( strTemp2 );
        }
        else if(match( MINUS )){
            advance();
            strTemp2 = term();
            printf("%s -= %s\n", strTemp, strTemp2 );
            releaseMem( strTemp2 );

        }
        else{
            flag=false;
        }
    }

    return strTemp;
}

char    *term()
{
    char  *strTemp, *strTemp2 ;

    strTemp = factor();
    bool flag=true; 
    while(flag)
    {
        if(match( TIMES )){
            advance();
            strTemp2 = factor();
            printf("%s *= %s\n", strTemp, strTemp2 );
            releaseMem( strTemp2 );
        }
        else if(match( DIV )){
            advance();
            strTemp2 = factor();
            printf("%s /= %s\n", strTemp, strTemp2 );
            releaseMem( strTemp2 );
        }
        else{
            flag=false;
        }
    }

    return strTemp;
}

char    *factor()
{
    char *strTemp=NULL;

    if( match(NUM_OR_ID))
    {
        if(isalpha(yytext[0]))
            printf("%s = _%.*s\n", strTemp = newname(), yyleng, yytext );
        else
            printf("%s = %.*s\n", strTemp = newname(), yyleng, yytext );
        advance();
    }
    else if( match(LP) )
    {
        advance();
        strTemp = expression();
        if( match(RP) )
            advance();
        else
            printf("%d: Mismatched parenthesis\n", yylineno );
    }
    else
    printf("%d: Number or identifier expected\n", yylineno );

    return strTemp;
}

int correctFutureBuffer( int first_arg, ... )
{

    va_list     args;
    int     tok;
    int     futureBuff[MAXFIRST], *p = futureBuff, *current;
    int     warningGiven = 0;
    int     toBeReturned          = 0;

    va_start( args, first_arg );

    if( !first_arg )
    {
        if( match(EOI) )
            toBeReturned = 1;
    }
    else
    {
        *p++ = first_arg;
        while( (tok = va_arg(args, int)) && p < &futureBuff[MAXFIRST] )
            *p++ = tok;

        while( !match( SYNCH ) ) {
            for( current = futureBuff; current < p ; ++current )
            if( match( *current ) )
            {
                toBeReturned = 1;
                goto exit;
            }

            if( !warningGiven ){
                fprintf( stderr, "Line %d: Syntax error\n", yylineno );
                warningGiven = 1;
            }
            advance();
       }
    }

exit:
    va_end( args );
    return toBeReturned;
}