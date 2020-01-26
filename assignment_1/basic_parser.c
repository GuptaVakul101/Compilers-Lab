/* Basic parser, shows the structure but there's no code generation */

#include <stdio.h>
#include "lex.h"

statements()
{
    /*  statements -> expression SEMI
     *             |  expression SEMI statements
     */

    expression();

    if( match( SEMI ) )
	look_forward();
    else
        fprintf( stderr, "%d: Inserting missing semicolon\n", line_number );

    if( !match(EOI) )
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

    if( match( PLUS ) )
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

    if( match( TIMES ) )
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

    if( match(NUM_OR_ID) )
        look_forward();

    else if( match(LP) )
    {
        look_forward();
        expression();
        if( match(RP) )
            look_forward();
        else
            fprintf( stderr, "%d: Mismatched parenthesis\n", line_number);
    }
    else
	fprintf( stderr, "%d Number or identifier expected\n", line_number );
}
