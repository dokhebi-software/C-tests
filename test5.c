#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <argp.h>
#include <stdbool.h>
#include "rnd.h"
#include "failtest.h"

/* ANSI colors */
#define BLUE  "\033[0;34m"
#define RED   "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"

/* argp definitions */
const char *argp_program_version = "test4 v1";
const char *argp_program_bug_address = "<dokhebi@gmail.com>";
static char doc[] = "Test of failtest routine with command line arguments.";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = {
	{ "count", 'c', "COUNT", 0, "Number of times to call failtest()" },
	{ "failcnd", 'f', "COND", 0, "Failure condition" },
	{ 0 }
};

struct arguments
{
	int count;
	int failcnd;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;
	switch (key)
	{
		case 'c': arguments->count = arg ? atoi(arg) : DEFCOUNT; break;
		case 'f': arguments->failcnd = arg ? atoi(arg) : DEFFAIL; break;
		case ARGP_KEY_ARG: return 0;
		default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char *argv[])
{
	struct arguments arguments;

	arguments.count = DEFCOUNT;
	arguments.failcnd = DEFFAIL;

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	/* int x = 10; */
	/* int count = DEFCOUNT; */

	/* Seed the random number generator. */
	srand(time(NULL));

	/* Start the whole thing off... */
	printf("%sGO%s", BLUE, RESET);

	/* Main loop... */
	for ( int lcv = 1; lcv <= arguments.count; lcv++ )
	{
		if (failtest(arguments.failcnd))
			printf(" %sSUCCESS%s", GREEN, RESET);
		else
			printf(" %sFAIL%s", RED, RESET);
	}

	/* Final message. */
	printf(" %sDONE!%s\n", BLUE, RESET);

	return 0;
}
