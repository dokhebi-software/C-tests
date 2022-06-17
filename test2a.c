#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "rnd.h"

int main()
{
	srand(time(NULL));
    for (int x = 0; x < 1000; x++)
    {
    	printf("%d\n", (int)(trunc(rnd()*999)+1));
    }
	return 0;
}
