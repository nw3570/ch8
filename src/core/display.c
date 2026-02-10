#include "ch8.h"
#include "ch8_internal.h"

uint8_t ch8_get_display_pixel(ch8_t *ch8, uint8_t x, uint8_t y)
{
    if (x >= CH8_DISPLAY_WIDTH || y >= CH8_DISPLAY_HEIGHT)
        return 0;

    return ch8->disp.pixels[y][x];
}

int ch8_display_needs_redraw(ch8_t *ch8)
{
    return ch8->disp.draw_flag;
}

void ch8_display_clear_redraw(ch8_t *ch8)
{
    ch8->disp.draw_flag = 0;
}

static void display_clear_pixels(uint8_t pixels[CH8_DISPLAY_HEIGHT][CH8_DISPLAY_WIDTH])
{
    uint8_t row, col;

    for (row = 0; row < CH8_DISPLAY_HEIGHT; row++)
        for (col = 0; col < CH8_DISPLAY_WIDTH; col++)
            pixels[row][col] = CH8_PIXEL_OFF;
}

void ch8_display_init(ch8display_t *disp)
{
    display_clear_pixels(disp->pixels);
    disp->draw_flag = 0;
}

void ch8_display_clear(ch8display_t *disp)
{
    display_clear_pixels(disp->pixels);
    disp->draw_flag = 1;
}

int ch8_display_draw(ch8display_t *disp, uint8_t x, uint8_t y, uint8_t n, const uint8_t *data)
{
    uint8_t row, col;
    uint8_t collision = 0;
    uint8_t x0 = x % CH8_DISPLAY_WIDTH;
    uint8_t y0 = y % CH8_DISPLAY_HEIGHT; 

    for (row = 0; row < n; row++) {
        if (row + y0 >= CH8_DISPLAY_HEIGHT)
            break;

        uint8_t sprite_row = data[row];

        for (col = 0; col < 8; col++) {
            if (col + x0 >= CH8_DISPLAY_WIDTH)
                break;

            uint8_t sprite_pixel = sprite_row & (0x80 >> col);
            uint8_t disp_row = y0 + row;

            if (sprite_pixel != 0) {
                uint8_t *pixel = &disp->pixels[disp_row][x0 + col];

                if (*pixel == CH8_PIXEL_ON)
                    collision = 1;

                *pixel ^= CH8_PIXEL_ON;
            }
        }
    }

    disp->draw_flag = 1;
    return collision;
}
