#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "rnd.h"

int main()
{
int x;

	srand(time(NULL));
	for (x=0; x<10; x++)
		printf("%3d", (int)trunc(rnd()*10) + 1);
	printf("\n");
	return 0;
}
