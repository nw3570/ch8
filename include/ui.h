#ifndef UI_H
#define UI_H

#include "ch8_defs.h"
#include <stdint.h>

#define UI_KEYS_FRAME_TIMEOUT 15
#define UI_KEYS_COUNT 16

typedef struct {
    uint8_t pressed;
    uint8_t frames_left;
} uikey_t;

void ui_setup();
void ui_end();

void ui_display_render(ch8_t *);

void ui_keys_update(uikey_t keys[UI_KEYS_COUNT]);

#endif // UI_H
