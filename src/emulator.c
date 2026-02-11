#include "ui.h"
#include "ch8.h"
#include <time.h>
#include <unistd.h>

#define NS_PER_FRAME 16666666l

static uint16_t keys_bitmask(uikey_t *keys)
{
    uint16_t mask = 0;

    for (uint8_t i = 0; i < CH8_KEYS_COUNT; i++) {
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
    struct timespec next_frame = { 0 };
    
    clock_gettime(CLOCK_MONOTONIC, &next_frame);

    while (1) {
        run_frame(machine, keys, cycles_per_frame);
        
        next_frame.tv_nsec += NS_PER_FRAME;

        if (next_frame.tv_nsec >= 1000000000L) {
            next_frame.tv_sec++;
            next_frame.tv_nsec -= 1000000000L;
        }

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_frame, NULL);
        //usleep(16666);
    }
}

void emulator_run(const char *program_path, int cpu_hz)
{
    ch8_t *machine = ch8_alloc();
    uikey_t keys[CH8_KEYS_COUNT] = { 0 };

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
