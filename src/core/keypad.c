#include "ch8_internal.h"
#include "ch8.h"

int ch8_key_is_pressed(uint16_t keys, uint8_t target)
{
    return (keys >> target) & 1;
}

int ch8_key_first_pressed(uint16_t keys, uint8_t *out_key)
{
    for (uint8_t key = 0; key < 16; key++) {
        if (ch8_key_is_pressed(keys, key)) {
            *out_key = key;
            return 1;
        }
    }

    return 0;
}

void ch8_set_keypad(ch8_t *ch8, uint16_t new_keys)
{
    ch8->keys = new_keys;    
}
