#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <argp.h>
#include <math.h>
#include "failtest.h"

/* Color pairs */
#define FAIL    1
#define SUCCESS 2
#define RUNNING 3
#define TEXT    4

/* argp definitions */
const char *argp_program_version = "test9 v1";
const char *argp_program_bug_address = "<dokhebi.software@gmail.com>";
static char doc[] = "Test of ncurses and pthreads.";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = {
    { "number-threads", 't', "NUMBEROFTHREADS", 0, "Number of threads to spawn.  Min is 1, max is 12, default is 4." },
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

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char *argv[])
{
    /* Thread ID number, used as a loop control variable. */
    long tid;

    /* Color pair */
    //int pair;

    /* Create the structure for the arguments. */
    struct arguments arguments;

    /* Box dimensions. */
    int startx, starty, width, height;
    
    /* Window handle. */
    WINDOW **testwin;
   
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
    if (( arguments.nthread < 1 ) || ( arguments.nthread > 12 ))
        arguments.nthread = DEFTHREAD;

    /* Create Window array. */
    testwin = malloc(sizeof(WINDOW)*arguments.nthread);
    
    /* Start curses mode. */
    initscr();

    /* Disable line buffering. */
    cbreak();

    /* Disable character echo. */
    noecho();

    /* Use the color settings from before program execution. */
    //use_default_colors();

    /* Initialize the colors */
    start_color();

    /* The definition of BLACK is wrong, fix it. */
    init_color(COLOR_BLACK, 0, 0, 0);

    /* Set the color pairs. */
    init_pair(FAIL, COLOR_RED, COLOR_BLACK);
    init_pair(SUCCESS, COLOR_GREEN, COLOR_BLACK);
    init_pair(RUNNING, COLOR_BLUE, COLOR_BLACK);
    init_pair(TEXT, COLOR_CYAN, COLOR_BLACK);

    /* Clear the screen. */
    clear();

    /* Define the dimensions of the box. */
    height = 5;
    width = 23;
    
    /* Refresh the screen. */
    refresh();
    
    for ( tid = 0; tid < arguments.nthread; tid++ )
    {
        /* Assign the upper-left corner based on thread number. */
        startx = 1 + ((tid  % 4) * 23);
        starty = 1 + (trunc(tid / 4) * 5);
        
        /* Create the window. */
        testwin[tid] = newwin(height, width, starty, startx);
    
        /* Populate it with the default border characters. */
        box(testwin[tid], 0, 0);
    
        /* Display message. */
        wattron(testwin[tid], COLOR_PAIR(TEXT));
        mvwprintw(testwin[tid], 0, 1, " Thread %d ", tid);
        wattroff(testwin[tid], COLOR_PAIR(TEXT));
    
        /* Display message. */
        wattron(testwin[tid], COLOR_PAIR(TEXT));
        mvwprintw(testwin[tid], 2, 2, "Process");
        wattroff(testwin[tid], COLOR_PAIR(TEXT));
        
        /* Change the color and display "failed." */
        wattron(testwin[tid], COLOR_PAIR(RUNNING));
        mvwprintw(testwin[tid], 2, 10, "running");
        wattroff(testwin[tid], COLOR_PAIR(RUNNING));

        /* Refresh the screen with the window. */
        wrefresh(testwin[tid]);
    
    }
    
    /* Grab a key stroke to exit. */
    getch();

    /* Clear the screen. */
    clear();

    /* Delete the curses window. */
    endwin();

    /* Exit and return success. */
    return 0;
}