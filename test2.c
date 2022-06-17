#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "rnd.h"

int main()
{
	srand(time(NULL));
	printf("%.16f\n", rnd());
	return 0;
}
