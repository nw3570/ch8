#include "ch8.h"
#include <stddef.h> // NULL
#include <ncurses.h> // WINDOW, mvaddch, refresh

void ch8_display_clear(ch8display_t *disp)
{
    uint8_t row, col;

    for (row = 0; row < CH8_DISPLAY_HEIGHT; row++)
        for (col = 0; col < CH8_DISPLAY_WIDTH; col++)
            disp->pixels[row][col] = CH8_DISPLAY_PIXEL_OFF;

    disp->draw_flag = 1;
}

void ch8_display_init(ch8display_t *disp)
{
    if (disp == NULL)
        return;

    ch8_display_clear(disp);
    disp->draw_flag = 0;
}

int ch8_display_draw(ch8display_t *disp, uint8_t x, uint8_t y, uint8_t n, const uint8_t *sprite_rows)
{
    uint8_t row, col;
    uint8_t collision = 0;
    uint8_t x0 = x % CH8_DISPLAY_WIDTH;
    uint8_t y0 = y % CH8_DISPLAY_HEIGHT; 

    for (row = 0; row < n; row++) {
        if (row + y0 >= CH8_DISPLAY_HEIGHT)
            break;

        uint8_t sprite_row = sprite_rows[row];

        for (col = 0; col < 8; col++) {
            if (col + x0 >= CH8_DISPLAY_WIDTH)
                break;

            uint8_t sprite_pixel = sprite_row & (0x80 >> col);
            uint8_t disp_row = y0 + row;

            if (sprite_pixel != 0) {
                uint8_t *pixel = &disp->pixels[disp_row][x0 + col];

                if (*pixel == CH8_DISPLAY_PIXEL_ON)
                    collision = 1;

                *pixel ^= CH8_DISPLAY_PIXEL_ON;
            }
        }
    }

    disp->draw_flag = 1;

    return collision;
}

void ch8_display_ncurses_render(ch8display_t *disp)
{
    if (!disp->draw_flag)
        return;

    for (int y = 0; y < CH8_DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < CH8_DISPLAY_WIDTH; x++) {

            char c = disp->pixels[y][x] ? '#' : ' ';

            mvaddch(y, x * 2, c);
            mvaddch(y, x * 2 + 1, c);
        }
    }

    refresh();
}
