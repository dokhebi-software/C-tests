#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rnd.h"
#include "failtest.h"

#define BLUE  "\033[0;34m"
#define RED   "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"

int main()
{
	int x = 10;
	int count = DEFCOUNT;

	/* Seed the random number generator. */
	srand(time(NULL));

	/* Start the whole thing off... */
	printf("%sGO%s", BLUE, RESET);

	/* Main loop... */
	for ( int lcv = 1; lcv <= count; lcv++ )
	{
		if (failtest(x))
			printf(" %sSUCCESS%s", GREEN, RESET);
		else
			printf(" %sFAIL%s", RED, RESET);
	}

	/* Final message. */
	printf(" %sDONE!%s\n", BLUE, RESET);

	return 0;
}
