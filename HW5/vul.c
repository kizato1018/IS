#include <stdio.h>

void fmtstr()
{
    char input[100];
    int var = 10;

    /* print out information for experiment purpose */
    printf( "Target address: 0x%x\n", (unsigned) &var );
    printf( "Data at target address: %d\n", var );

    printf( "Please enter a string: " );
    fgets( input, sizeof(input), stdin );

    printf( input ); // The vulnerable place   

    printf( "Data at target address: %d\n", var );
}

void main() { fmtstr(); }
