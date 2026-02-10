#include <stdlib.h>
#include <string.h>
#include "ch8.h"
#include "ch8_internal.h"

static void ch8_init(ch8_t *ch8)
{
    if (!ch8)
        return;

    ch8_cpu_init(&ch8->cpu);
    ch8_display_init(&ch8->disp);
    memset(ch8->mem , 0, sizeof (uint8_t) * CH8_MEMORY_SIZE);

    //ch8->sound_timer = 0;
    ch8->delay_timer = 0;
}

ch8_t *ch8_alloc()
{
    ch8_t *ch8 = malloc(sizeof (ch8_t));

    if (ch8)
        ch8_init(ch8);

    return ch8;
}

void ch8_free(ch8_t *ch8)
{
    free(ch8);
}

void ch8_emulate_cycle(ch8_t *ch8)
{
    uint16_t opcode = ch8_fetch_instruction(&ch8->cpu, ch8->mem);
    ch8_execute_instruction(ch8, opcode);
}

void ch8_tick_timers(ch8_t *ch8)
{
    if (ch8->delay_timer > 0)
        ch8->delay_timer--;

    //if (ch8->sound_timer > 0)
    //    ch8->sound_timer--;
}
