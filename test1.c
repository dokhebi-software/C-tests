#include <stdio.h>
#include <time.h>
#include <locale.h>

int main()
{
	setlocale(LC_NUMERIC, "");
	printf("%'ld\n", time(NULL));
	return 0;
}
