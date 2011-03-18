#include <ncurses.h>
#include <signal.h>

#include <cassert>

#include <iostream>

#include "tetris.h"

WINDOW *GTileboardWindow;

void initInterface();
void destroyInterface();

void drawInterface();
void drawTileboard();

int main( int argc, char* argv[] )
{
    initscr();
    keypad( stdscr, true );         // enable numpad
    cbreak();                       // do not buffer input (wait for \n)
    noecho();                         // echo input in color
    timeout(0);

    initInterface();

    if ( has_colors() )
    {
        start_color();

        init_pair( 1, COLOR_RED, COLOR_BLACK );
        init_pair( 2, COLOR_GREEN, COLOR_BLACK );
        init_pair( 3, COLOR_YELLOW, COLOR_BLACK );
        init_pair( 4, COLOR_BLUE, COLOR_BLACK );
        init_pair( 5, COLOR_CYAN, COLOR_BLACK );
        init_pair( 6, COLOR_MAGENTA, COLOR_BLACK );
        init_pair( 7, COLOR_WHITE, COLOR_BLACK );
    }

    while ( true )
    {
        int c = getch();

        drawInterface();
        drawTileboard();

        // draw it
//        move( 2, 50 );
//        addch( c );

//        refresh();
    }

    destroyInterface();

    return 0;
}

void initInterface()
{
    GTileboardWindow = newwin( 20, 10, 1, 1 );
    assert( GTileboardWindow != NULL );

    box( GTileboardWindow, 0, 0 );
    wrefresh( GTileboardWindow );
}

void destroyInterface()
{
    wborder( GTileboardWindow, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh( GTileboardWindow );
    delwin( GTileboardWindow );
}

void drawInterface()
{
    assert( GTileboardWindow != NULL );

    wrefresh( GTileboardWindow );
//    move( 2, 60 );
//    printw( "hello world" );
}

void drawTileboard()
{

}
