char  *registers[] = { "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7" };   
char  **register_p  = registers;   
   
char  *new_register()   
{   
    if( register_p >= &registers[ sizeof(registers)/sizeof(*registers) ] )   
    {   
        fprintf( stderr, "%d: Expression too complex\n", line_number );   
        exit(1);   
    }   
   
    return( *register_p++ );   
}   
   
void releaseMem(s)   
char *s;   
{   
    if( register_p > registers )   
    {
        *--register_p = s;   
    }
    else
    {
        fprintf(stderr, "%d: (Internal error) Name stack underflow\n",line_number );   
    }   
}   
