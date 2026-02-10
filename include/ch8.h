#ifndef CH8_H
#define CH8_H

#include <stddef.h> // size_t
#include <stdint.h>
#include "ch8_defs.h"

ch8_t  *ch8_alloc();
void    ch8_free(ch8_t *);
size_t  ch8_load_program(ch8_t *, const char *path);
void    ch8_emulate_cycle(ch8_t *);
void    ch8_tick_timers(ch8_t *);
uint8_t ch8_get_display_pixel(ch8_t *, uint8_t x, uint8_t y);
int     ch8_display_needs_redraw(ch8_t *);
void    ch8_display_clear_redraw(ch8_t *);
//void   ch8_set_keypad(ch8_t *, uint16_t new_keys);

#endif // CH8_H
