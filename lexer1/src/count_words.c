#include <stdio.h>
#include <stdlib.h>
#include <counter.h>
#include <lexer.h>

int main(int argc, char **argv)
{
    int counts[4];

    counter( counts );
    printf( "%d %d %d %d\n", counts[0], counts[1], counts[2], counts[3] );
    exit(0);
}
