/* Basic parser, shows the structure but there's no code generation */

#include <stdio.h>
#include "lex.h"

statements()
{
    /*  statements -> expression SEMI
     *             |  expression SEMI statements
     */

    expression();

    if( common_check( SEMI ) )
	look_forward();
    else
        fprintf( stderr, "%d: Inserting missing semicolon\n", line_number );

    if( !common_check(EOI) )
        statements();			/* Do another statement. */
}

expression()
{
    /* expression -> term expression' */

    term();
    expr_prime();
}

expr_prime()
{
    /* expression' -> PLUS term expression'
     *              | epsilon
     */

    if( common_check( PLUS ) )
    {
        look_forward();
        term();
        expr_prime();
    }
}

term()
{
    /* term -> factor term' */

    factor();
    term_prime();
}

term_prime()
{
    /* term' -> TIMES factor term'
     *       |   epsilon
     */

    if( common_check( TIMES ) )
    {
        look_forward();
        factor();
        term_prime();
    }
}

factor()
{
    /* factor   ->    NUM_OR_ID
     *          |     LP expression RP
     */

    if( common_check(NUM_OR_ID) )
        look_forward();

    else if( common_check(LP) )
    {
        look_forward();
        expression();
        if( common_check(RP) )
            look_forward();
        else
            fprintf( stderr, "%d: Miscommon_checked parenthesis\n", line_number);
    }
    else
	fprintf( stderr, "%d Number or identifier expected\n", line_number );
}
