#include "ui.h"
#include "ch8.h"
#include <unistd.h>

#define EMULATOR_LOOP_DELAY 16666

static uint16_t keys_bitmask(uikey_t *keys)
{
    uint16_t mask = 0;

    for (int i = 0; i < UI_KEYS_COUNT; i++) {
        if (keys[i].pressed)
            mask |= (1 << i);
    }

    return mask;
}

static void run_frame(ch8_t *machine, uikey_t *keys, int cycles_per_frame)
{
    ui_keys_update(keys);
    uint16_t keypad_state = keys_bitmask(keys);
    ch8_set_keypad(machine, keypad_state);
        
    for (int c = 0; c < cycles_per_frame; c++) {
        ch8_emulate_cycle(machine);
    }

    ch8_tick_timers(machine);

    if (ch8_display_needs_redraw(machine)) {
        ui_display_render(machine);
        ch8_display_clear_redraw(machine);
    }   
}

static void emulator_loop(ch8_t *machine, uikey_t *keys, int cpu_hz)
{
    const int cycles_per_frame = cpu_hz / 60;
    
    while (1) {
        run_frame(machine, keys, cycles_per_frame);
        usleep(16666);
    }
}

void emulator_run(const char *program_path, int cpu_hz)
{
    ch8_t *machine = ch8_alloc();
    uikey_t keys[UI_KEYS_COUNT] = { 0 };

    if (!machine)
        return;

    if (ch8_load_program(machine, program_path) == 0) {
        ch8_free(machine);
        return;
    }

    ui_setup();
    emulator_loop(machine, keys, cpu_hz);

    ui_end();
    ch8_free(machine);
}
