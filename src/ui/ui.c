#include <ncurses.h>
#include "ui.h"
#include "ch8.h"

void ui_setup()
{
    initscr();
    cbreak();
    nodelay(stdscr, 1);
    curs_set(0);
}

void ui_end()
{
    endwin();
}

void ui_display_render(ch8_t *machine)
{
      for (uint8_t y = 0; y < CH8_DISPLAY_HEIGHT; y++) {
        for (uint8_t x = 0; x < CH8_DISPLAY_WIDTH; x++) {
            uint8_t pixel = ch8_get_display_pixel(machine, x, y);
            char c = pixel == CH8_PIXEL_ON ? '#' : ' ';

            mvaddch(y, x * 2, c);
            mvaddch(y, x * 2 + 1, c);
        }
    }

    refresh();
}