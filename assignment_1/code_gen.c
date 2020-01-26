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
//int correctFutureBuffer( int num_args  , ...);

void statements()
{
    while(!common_check(EOI))
    {
        stmnt();
    }
}

void confirm_semiColon()
{
    if( common_check( SEMI ) )
        look_forward();
    else
        printf("%d: Inserting missing semicolon\n", line_number );
}

void stmnt()
{
    /*  statements -> expression SEMI  |  expression SEMI statements  */
        char *my_str;
        if(common_check(ID))
        {
            char array[100];
            for(int i=0;i<cur_size;i++)
                array[i]=*(cur_para+i);

            array[cur_size]='\0';

            look_forward();  
            if(!correctFutureBuffer( COL) )
            {
                printf("%d: Semi Colon is missing at this line: \n", line_number );
                confirm_semiColon();
            }
            else{
                look_forward();
                if( !correctFutureBuffer( EQUAL ) )
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
        else if(common_check(IF)){
            look_forward();
            printf("if (\n");

            my_str = comparator();

            if( !correctFutureBuffer( THEN) )
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
                stmnt();
                printf("}\n");
                return;
            }
        }
        else if(common_check(WHILE))
        {
            look_forward();
            printf("while (\n");
            my_str = comparator();
            if( !correctFutureBuffer( DO) )
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
                stmnt();
                printf("}\n");
                return;
            }
        }
        else if(common_check(BEGIN))
        {
            printf("BEGIN{\n");

            look_forward();
            list_of_statements();
            if(!correctFutureBuffer(END))
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
        confirm_semiColon:
            if( common_check( SEMI ) )
                look_forward();
            else
                printf("%d: Semicolon missing at this line\n", line_number );
}

void list_of_statements()
{
    while(1)
    {
        if(common_check(END) || common_check(EOI))
        {
            break;
        }
        stmnt();
    }
    if(common_check(EOI))
    {
        fprintf( stderr, "%d:The file has been completed. END statement not found\n", line_number );
    }
}

char *comparator()
{
    char *my_str3;
    char *my_str=expression();
    if( common_check(GT) )
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
    else if(common_check(LT))
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
    else if(common_check(EQUAL))
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
        if(common_check( PLUS ))
        {
            look_forward();
            my_str2 = term();

            printf("%s += %s\n", my_str, my_str2 );
            releaseMem( my_str2 );
        }
        else if(common_check( MINUS ))
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
        int f1 = common_check(TIMES);
        int f2 = common_check(DIV);

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

    if( common_check(NUM_OR_ID))
    {
        if(isalpha(cur_para[0]))
            printf("%s = _%.*s\n", my_str = new_register(), cur_size, cur_para );
        else
            printf("%s = %.*s\n", my_str = new_register(), cur_size, cur_para );
        look_forward();
    }
    else if( common_check(LP) )
    {
        look_forward();
        my_str = expression();
        if( common_check(RP) )
            look_forward();
        else
            printf("%d: Brackets are not common_checking\n", line_number );
    }
    else
    {
        printf("%d: Some variable name or constant number expected\n", line_number );
    }

    return my_str;
}


int correctFutureBuffer(int f_arg)
{
    int warningGiven = 0;

    if(f_arg == 0)
    {
        if( common_check(EOI) )
            return 1;
    }
    else
    {
        while( !common_check( SEMI ) ) 
        {
            if( common_check( f_arg ) )
                return 1;

            if( !warningGiven )
            {
                fprintf( stderr, "Line %d: Syntax error\n", line_number );
                warningGiven = 1;
            }
            look_forward();
       }
    }

    return 0;
}


/*int correctFutureBuffer( int num_args, ... )
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
        if( common_check(EOI) )
        {
            toBeReturned = 1;
        }
    }
    else
    {
        *p++ = num_args;
        while( (my_x = va_arg(var_list, int)) && p < &futureBuff[MAX_SIZE] )
            *p++ = my_x;

        while( !common_check( SEMI ) ) {
            for( current = futureBuff; current < p ; ++current )
            if( common_check( *current ) )
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
}*/