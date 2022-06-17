#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <argp.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>
#include <curses.h>
#include <string.h>
#include <unistd.h>
#include "rnd.h"
#include "failtest.h"

/* Color pairs */
#define FAIL    1
#define SUCCESS 2
#define RUNNING 3

/* argp definitions */
const char *argp_program_version = "test7 v1";
const char *argp_program_bug_address = "<dokhebi.software@gmail.com>";
static char doc[] = "Test of ncurses and pthreads.";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = {
    { "number-threads", 't', "NUMBEROFTHREADS", 0, "Number of threads to spawn.  Min is 1, max is 8, default is 4." },
    { "fail-condition", 'f', "FAILCONDITION", 0, "Failure condition.  Must be positive, default is 100." },
    { "base-runtime", 'B', "BASERUNTIME", 0, "Base runtime for each thread.  Default is 25." },
    { "runtime-variance", 'R', "RUNTIMEVARIANCE", 0, "Variance to base runtime.  Default is 5 (+/-5)." },
    { 0 }
};

struct arguments
{
    int nthread;
    int failcnd;
    int brun;
    int runv;
};

struct threadargs
{
    long tid;
    int rc;
    int runtime;
    int failcnd;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;
    switch (key)
    {
        case 't': arguments->nthread = arg ? abs(atoi(arg)) : DEFTHREAD; break;
        case 'f': arguments->failcnd = arg ? abs(atoi(arg)) : DEFFAIL; break;
        case 'B': arguments->brun = arg ? abs(atoi(arg)) : DEFBRUN; break;
        case 'R': arguments->runv = arg ? abs(atoi(arg)) : DEFRUNV; break;
        case ARGP_KEY_ARG: return 0;
        default: return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

void *testthread(void *input)
{
    /* Initialize local variables. */
    int rc = 0;

    /* Loop until "runtime" or the test fails.*/
    for (int x = 0; x < ((struct threadargs*)input)->runtime; x++)
    {
        if (failtest(((struct threadargs*)input)->failcnd))
        {
            rc = 1;
        }
        else
        {
            rc = -1;
            break;
        }
    }

    /* Sleep for a few seconds... */
    sleep((int)trunc(rnd()*3)+1);

    /* Update the return code for the calling thread. */
    ((struct threadargs*)input)->rc = rc;

    /* Exit the thread. */
    pthread_exit(NULL);
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char *argv[])
{
    /* Thread ID number, used as a loop control variable. */
    long tid;

    /* Color pair */
    int pair;

    /* Pointer to threads array. */
    pthread_t *threads;

    /* Pointer to threadargs array. */
    struct threadargs *threadargs;

    /* Create the structure for the arguments. */
    struct arguments arguments;

    /* Buffer for sprintf. */
    char buffer[15];

    /* Number of threads to wait for status. */
    int numwait;

    /* Seed the random number generator. */
    srand(time(NULL));

    /* Set the default values. */
    arguments.nthread = DEFTHREAD;
    arguments.failcnd = DEFFAIL;
    arguments.brun    = DEFBRUN;
    arguments.runv    = DEFRUNV;

    /* Parse the command line. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    /* Check to make sure the number of threads is reasonable. */
    /* It can't be 0 and 9 is to many.                         */
    /* Set to DEFTHREAD if outside the range.                  */
    if (( arguments.nthread < 1 ) || ( arguments.nthread > 8 ))
        arguments.nthread = DEFTHREAD;

    /* Allocate memory for threads array. */
    threads  = malloc(sizeof(pthread_t)*arguments.nthread);

    /* Allocate memory for the arg array. */
    threadargs = calloc(arguments.nthread, sizeof(struct threadargs));

    /* Initialize thread arguments array. */
    for ( tid = 0; tid < arguments.nthread; tid++ )
    {
        threadargs[tid].tid = (long)tid;
        threadargs[tid].rc = 0;
        threadargs[tid].runtime = arguments.brun + ((int)(trunc(rnd()*arguments.runv*2)+1) - arguments.runv); 
        threadargs[tid].failcnd = arguments.failcnd;
    }

    /* Initialize curses. */
    initscr();
    cbreak();
    noecho();
    use_default_colors();

    /* Initialize the colors */
    start_color();
    init_color(COLOR_BLACK, 0, 0, 0);              // Fix color black
    //init_color(COLOR_RED, 255, 0, 0);              // Fix color red
    init_pair(FAIL, COLOR_RED, COLOR_BLACK);
    init_pair(SUCCESS, COLOR_GREEN, COLOR_BLACK);
    init_pair(RUNNING, COLOR_BLUE, COLOR_BLACK);

    /* Clear the screen. */
    clear();

    /* Print the command line arguments. */
    mvprintw(3, 2, "# of threads            - %3d", arguments.nthread);
    mvprintw(4, 2, "fail condition          - %3d", arguments.failcnd);
    mvprintw(5, 2, "Thread base runtime     - %3d", arguments.brun);
    mvprintw(6, 2, "Thread runtime variance - %3d", arguments.runv);

    /* Refresh the screen. */
    refresh();

    /* Launch the threads. */
    for ( tid = 0; tid < arguments.nthread; tid++ )
    {
        sleep(0);
        mvprintw(8+tid, 2, "Thread #%ld: runtime %d", tid, threadargs[tid].runtime);
        pthread_create(&threads[tid], NULL, testthread, (void *)&threadargs[tid]);
    }

    sleep(1);

    /* Show the status messages. */
    for ( tid = 0; tid < arguments.nthread; tid++)
    {
        mvprintw(25+tid, 2, "Thread #%d", (int)threadargs[tid].tid);
    }

    /* Print return codes. */
    do
    {
        numwait = 0;
        for ( tid = 0; tid < arguments.nthread; tid++ )
        {
            switch (threadargs[tid].rc)
            {
                case  1: sprintf(buffer, "%s", "succeeded"); pair = SUCCESS; break;
                case -1: sprintf(buffer, "%s", "failed   "); pair = FAIL; break;
                default: sprintf(buffer, "%s", "running  "); pair = RUNNING; numwait++;
            }
            move(25+tid, 12);
            attron(COLOR_PAIR(pair));
            printw(buffer);
            attroff(COLOR_PAIR(pair));
            refresh();
        }
        sleep(1);
    } while (numwait);

    /* Put in a pause. */
    mvprintw((LINES - 1), 2, "Press any key to quit");

    /* Refresh the screen. */
    refresh();

    /* Grab a key stroke to exit. */
    getch();

    /* Delete the curses window. */
    endwin();

    /* Free up the memory allocated to the thread arguments and the ???. */
    free(threadargs);
    free(threads);

    return 0;
}
