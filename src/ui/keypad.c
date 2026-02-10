#include <string.h>
#include <ncurses.h>
#include "ui.h"

static int key_map(char c)
{
    switch (c) {
        case '1': return 0x1;
        case '2': return 0x2;
        case '3': return 0x3;
        case '4': return 0xC;
        case 'q': return 0x4;
        case 'w': return 0x5;
        case 'e': return 0x6;
        case 'r': return 0xD;
        case 'a': return 0x7;
        case 's': return 0x8;
        case 'd': return 0x9;
        case 'f': return 0xE;
        case 'z': return 0xA;
        case 'x': return 0x0;
        case 'c': return 0xB;
        case 'v': return 0xF;
    }

    return -1;
}

static void get_keys(uikey_t keys[UI_KEYS_COUNT])
{
    char c;

    while ((c = getch()) != ERR) {
        int key = key_map(c);

        if (key > 0) {
            keys[key].pressed = 1;
            keys[key].frames_left = UI_KEYS_FRAME_TIMEOUT;
        }
    }
}

static void update_key(uikey_t *key)
{
    if (!key->pressed)
        return;

    if (key->frames_left > 0)
        key->frames_left--;
    
    if (key->frames_left == 0)
        key->pressed = 0;
}

void ui_keys_update(uikey_t keys[UI_KEYS_COUNT])
{
    get_keys(keys);

    for (int i = 0; i < UI_KEYS_COUNT; i++) {
        update_key(&keys[i]);
    }
}