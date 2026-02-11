#include <ncurses.h>
#include "ui.h"
#include "ch8.h"

void ui_setup()
{
    initscr();
    cbreak();
    noecho();
    timeout(0);
    curs_set(0);
    leaveok(stdscr, 1);
}

void ui_end()
{
    endwin();
}

void ui_display_render(ch8_t *machine)
{
    erase();

    for (uint8_t y = 0; y < CH8_DISPLAY_HEIGHT; y++) {
        for (uint8_t x = 0; x < CH8_DISPLAY_WIDTH; x++) {
            uint8_t pixel = ch8_get_display_pixel(machine, x, y);
            char c = pixel == CH8_PIXEL_ON ? '#' : ' ';

            mvaddch(y, x * 2, c);
            mvaddch(y, x * 2 + 1, c);
        }
    }

    wnoutrefresh(stdscr);
    doupdate();
}