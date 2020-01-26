/* Revised parser  */

#include <stdio.h>
#include "lex.h"

void    factor      ( void );
void    term        ( void );
void    expression  ( void );

statements()
{
    /*  statements -> expression SEMI |  expression SEMI statements */

    while( !common_check(EOI) )
    {
        expression();

        if( common_check( SEMI ) )
            look_forward();
        else
            fprintf( stderr, "%d: Inserting missing semicolon\n", line_number );
    }
}

void    expression()
{
    /* expression  -> term expression'
     * expression' -> PLUS term expression' |  epsilon
     */

    if( !legal_next_tok( NUM_OR_ID, LP, 0 ) )
	return;

    term();
    while( common_check( PLUS ) )
    {
        look_forward();
        term();
    }
}

void    term()
{
    if( !legal_next_tok( NUM_OR_ID, LP, 0 ) )
	return;

    factor();
    while( common_check( TIMES ) )
    {
        look_forward();
        factor();
    }
}

void    factor()
{
    if( !legal_next_tok( NUM_OR_ID, LP, 0 ) )
	return;

    if( common_check(NUM_OR_ID) )
        look_forward();

    else if( common_check(LP) )
    {
        look_forward();
        expression();
        if( common_check(RP) )
            look_forward();
        else
            fprintf( stderr, "%d: Miscommon_checked parenthesis\n", line_number );
    }
    else
	fprintf( stderr, "%d: Number or identifier expected\n", line_number );
}
#include <stdarg.h>

#define MAX_SIZE 16
#define SEMI	 SEMI

int	legal_next_tok(  first_arg )
int	first_arg;
{
    /* Simple error detection and recovery. Arguments are a 0-terminated list of
     * those tokens that can legitimately come next in the input. If the list is
     * empty, the end of file must come next. Print an error message if
     * necessary. Error recovery is performed by discarding all input symbols
     * until one that's in the input list is found
     *
     * Return true if there's no error or if we recovered from the error,
     * false if we can't recover.
     */

    va_list  	args;
    int		tok;
    int		next_toks[MAX_SIZE], *p = next_toks, *current;
    int		error_printed = 0;
    int		rval	      = 0;

    va_start( args, first_arg );

    if( !first_arg )
    {
	if( common_check(EOI) )
	    rval = 1;
    }
    else
    {
	*p++ = first_arg;
	while( (tok = va_arg(args, int)) && p < &next_toks[MAX_SIZE] )
	    *p++ = tok;

	while( !common_check( SEMI ) )
	{
	    for( current = next_toks; current < p ; ++current )
		if( common_check( *current ) )
		{
		    rval = 1;
		    goto exit;
		}

	    if( !error_printed )
	    {
		fprintf( stderr, "Line %d: Syntax error\n", line_number );
		error_printed = 1;
	    }

	    look_forward();
	}
    }

exit:
    va_end( args )
    return rval;
}
