#include <stdio.h>
#include <curses.h>

/* Color pairs */
#define FAIL    1
#define SUCCESS 2
#define RUNNING 3

int main(int argc, char *argv[])
{
    /* Buffer for sprintf. */
    //char buffer[30];

    /* Box dimensions. */
    int startx, starty, width, height;
    
    /* Window handle. */
    WINDOW *testwin;

    /* Start curses mode. */
    initscr();

    /* Disable line buffering. */
    cbreak();

    /* Disable character echo. */
    noecho();

    /* Use the color settings from before program execution. */
    use_default_colors();

    /* Initialize the colors */
    start_color();

    /* The definition of BLACK is wrong, fix it. */
    init_color(COLOR_BLACK, 0, 0, 0);

    /* Set the color pairs. */
    init_pair(FAIL, COLOR_RED, COLOR_BLACK);
    init_pair(SUCCESS, COLOR_GREEN, COLOR_BLACK);
    init_pair(RUNNING, COLOR_BLUE, COLOR_BLACK);

    /* Clear the screen. */
    clear();

    /* Define the dimensions of the box. */
    height = 5;
    width = 23;
    starty = 1;  // Calculating for a center placement
    startx = 1;    // of the window.
    
    /* Refresh the screen. */
    refresh();
    
    /* Create the window. */
    testwin = newwin(height, width, starty, startx);
    
    /* Populate it with the default border characters. */
    box(testwin, 0, 0);
    
    /* Display message. */
    mvwprintw(testwin, 0, 1, " Thread %d ", 0);
    
    /* Display message. */
    mvwprintw(testwin, 2, 2, "Process");
        
    /* Change the color and display "failed." */
    wattron(testwin, COLOR_PAIR(FAIL));
    mvwprintw(testwin, 2, 10, "failed");
    wattroff(testwin, COLOR_PAIR(FAIL));

    /* Refresh the screen with the window. */
    wrefresh(testwin);
    
    /* Grab a key stroke to exit. */
    getch();

    /* Clear the screen. */
    clear();

    /* Delete the curses window. */
    endwin();

    /* Exit and return success. */
    return 0;
}
