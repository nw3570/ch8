#include "ch8_internal.h"
#include "ch8.h"

uint8_t ch8_key_is_pressed(uint16_t keys, uint8_t target)
{
    return (keys >> target) & 1;
}

uint8_t ch8_key_first_pressed(uint16_t keys)
{
    uint8_t key;

    for (key = 0; key < 16; key++) {
        if (ch8_key_is_pressed(keys, key))
            return key;
    }

    return 0xff;
}

void ch8_set_keypad(ch8_t *ch8, uint16_t new_keys)
{
    ch8->keys = new_keys;    
}
