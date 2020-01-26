#include <stdio.h>
#include "lex.h"
#include "lex.c"
#include "name.c"
#include <stdbool.h>

#include <stdarg.h>

#define MAX_SIZE 16

char *factor( void );
char *term( void );
char *expression( void );

extern char *new_register(void);
extern void releaseMem( char *name );

char *comparator();
int correctFutureBuffer( int num_args  , ...);

void statements()
{
    while(!match(EOI))
    {
        statement();
    }
}

void confirm_semiColon()
{
    if( match( SEMI ) )
        look_forward();
    else
        printf("%d: Inserting missing semicolon\n", line_number );
}

void statement()
{
    /*  statements -> expression SEMI  |  expression SEMI statements  */
        char *my_str;
        if(match(ID))
        {
            char array[100];
            for(int i=0;i<cur_size;i++)
                array[i]=*(cur_para+i);

            array[cur_size]='\0';

            look_forward();  
            if(!correctFutureBuffer( COL, 0 ) )
            {
                printf("%d: Semi Colon is missing at this line: \n", line_number );
                confirm_semiColon();
            }
            else{
                look_forward();
                if( !correctFutureBuffer( EQUAL, 0 ) )
                {
                    printf("%d: Equal sign missing at this line\n", line_number );
                    confirm_semiColon();
                }
                else
                {
                    look_forward();
                    my_str = comparator();
                    printf("_%s <- %s\n",array,my_str);
                }
            }
        }

        else if(match(IF)){
            look_forward();
            printf("if (\n");

            my_str = comparator();

            if( !correctFutureBuffer( THEN, 0 ) )
            {
                releaseMem(my_str);
                printf("%d: Then sign missing at this line\n", line_number );
                confirm_semiColon();
            }
            else
            {
                printf("%s)\nthen {\n", my_str);
                look_forward();
                releaseMem(my_str);
                statement();
                printf("}\n");
                return;
            }
        }
        else if(match(WHILE))
        {
            look_forward();
            printf("while (\n");
            my_str = comparator();
            if( !correctFutureBuffer( DO, 0 ) )
            {
                releaseMem(my_str);
                printf("%d: Do sign missing at this line\n", line_number );
                confirm_semiColon();
            }
            else
            {
                printf("%s)\n do {\n", my_str);
                look_forward();
                releaseMem(my_str);
                statement();
                printf("}\n");
                return;
            }
        }
        else if(match(BEGIN))
        {
            printf("BEGIN{\n");

            look_forward();
            list_of_statements();
            if(!correctFutureBuffer(END,0))
            {
                printf("%d: END missing at this line\n", line_number );
                confirm_semiColon();
            } 
            else 
            {
                printf("} END\n");
                look_forward();
            }
            return;
        }
        else{
            my_str = comparator();
        }


        if(my_str)
        {
            releaseMem( my_str );
        } 
        else
        {
            exit(1);
        }
        // my_str = expression();
        confirm_semiColon:
            if( match( SEMI ) )
                look_forward();
            else
                printf("%d: Semicolon missing at this line\n", line_number );

        // releaseMem( my_str );
}

void list_of_statements()
{
    while(1)
    {
        if(match(END) || match(EOI))
        {
            break;
        }
        statement();
    }
    if(match(EOI))
    {
        fprintf( stderr, "%d:The file has been completed. END statement not found\n", line_number );
    }
}

char *comparator()
{
    char *my_str3;
    char *my_str=expression();
    if( match(GT) )
    {
        releaseMem(my_str);
        my_str3=new_register();
        my_str=new_register();

        printf("%s <- %s\n", my_str, my_str3);

        look_forward();
        char *my_str2=expression();
        printf("%s <-  %s > %s\n",my_str3,my_str,my_str2);
        
        releaseMem(my_str2);
        releaseMem(my_str);

        return my_str3;
    }
    else if(match(LT))
    {
        releaseMem(my_str);
        my_str3=new_register();
        my_str=new_register();

        printf("%s <- %s\n", my_str, my_str3);

        look_forward();
        char *my_str2=expression();
        printf("%s <-  %s < %s\n",my_str3,my_str,my_str2);

        releaseMem(my_str);
        releaseMem(my_str2);
        return my_str3;
    }
    else if(match(EQUAL))
    {
        releaseMem(my_str);
        my_str3=new_register();
        my_str=new_register();

        printf("%s <- %s\n", my_str, my_str3);

        look_forward();
        char *my_str2=expression();
        printf("%s <-  %s == %s\n",my_str3,my_str,my_str2);

        releaseMem(my_str);
        releaseMem(my_str2);
        return my_str3;
    }
    return my_str;
}

char *expression()
{
    char *my_str, *my_str2;
    my_str = term();
    bool flag = true;

    while(flag)
    {
        if(match( PLUS ))
        {
            look_forward();
            my_str2 = term();

            printf("%s += %s\n", my_str, my_str2 );
            releaseMem( my_str2 );
        }
        else if(match( MINUS ))
        {
            look_forward();
            my_str2 = term();

            printf("%s -= %s\n", my_str, my_str2 );
            releaseMem( my_str2 );
        }
        else
        {
            flag = false;
        }
    }

    return my_str;
}

char *term()
{
    char  *my_str, *my_str2;
    my_str = factor();

    while(1)
    {
        int f1 = match(TIMES);
        int f2 = match(DIV);

        if(f1 || f2)
        {
            look_forward();
            my_str2 = factor();

            if(f1)
            {
                printf("%s *= %s\n", my_str, my_str2 );
            }
            else
            {
                printf("%s /= %s\n", my_str, my_str2 );
            }
            releaseMem( my_str2 );
        }
        else
        {
            break;
        }
    }

    return my_str;
}

char *factor()
{
    char *my_str=NULL;

    if( match(NUM_OR_ID))
    {
        if(isalpha(cur_para[0]))
            printf("%s = _%.*s\n", my_str = new_register(), cur_size, cur_para );
        else
            printf("%s = %.*s\n", my_str = new_register(), cur_size, cur_para );
        look_forward();
    }
    else if( match(LP) )
    {
        look_forward();
        my_str = expression();
        if( match(RP) )
            look_forward();
        else
            printf("%d: Brackets are not matching\n", line_number );
    }
    else
    {
        printf("%d: Some variable name or constant number expected\n", line_number );
    }

    return my_str;
}





int correctFutureBuffer( int num_args, ... )
{

    va_list var_list;
    int my_x;
    int futureBuff[MAX_SIZE];
    int *p = futureBuff; 
    int *current;
    int warningGiven = 0;
    int toBeReturned = 0;

    va_start( var_list, num_args );

    if( num_args == 0 ) //that is the value of num_args given is 0 (corresponds to EOI)
    {
        if( match(EOI) )
        {
            toBeReturned = 1;
        }
    }
    else
    {
        *p++ = num_args;
        while( (my_x = va_arg(var_list, int)) && p < &futureBuff[MAX_SIZE] )
            *p++ = my_x;

        while( !match( SEMI ) ) {
            for( current = futureBuff; current < p ; ++current )
            if( match( *current ) )
            {
                toBeReturned = 1;
                va_end( var_list );
                return toBeReturned;
            }

            if( !warningGiven ){
                fprintf( stderr, "Line %d: Syntax error\n", line_number );
                warningGiven = 1;
            }
            look_forward();
       }
    }

    va_end( var_list );
    return toBeReturned;
}