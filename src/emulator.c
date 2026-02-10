#include "ui.h"
#include "ch8.h"
#include <stddef.h>
#include <time.h>

static double now_sec()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec + (double) ts.tv_nsec / 1e9;
}

static void emulator_loop(ch8_t *machine, int cpu_hz)
{
    const double cpu_period = 1.0 / cpu_hz;
    const double timers_period = 1.0 / 60.0;

    double cpu_time_acc = 0.0;
    double timers_time_acc = 0.0;
    double last_time = now_sec();

    while (1) {
        // ui_set_keypad_state(machine);

        double current_time = now_sec();
        double delta = current_time - last_time;
        last_time = current_time;

        cpu_time_acc += delta;
        timers_time_acc += delta;

        while (cpu_time_acc >= cpu_period) {
            ch8_emulate_cycle(machine);
            cpu_time_acc -= cpu_period;
        }

        while (timers_time_acc >= timers_period) {
            ch8_tick_timers(machine);
            
            if (ch8_display_needs_redraw(machine)) {
                ui_display_render(machine);
                ch8_display_clear_redraw(machine);
            }

            timers_time_acc -= timers_period;
        }  
    }
}

void emulator_run(const char *program_path, int cpu_hz)
{
    ch8_t *machine = ch8_alloc();

    if (!machine)
        return;

    if (ch8_load_program(machine, program_path) == 0) {
        ch8_free(machine);
        return;
    }

    ui_setup();
    emulator_loop(machine, cpu_hz);

    ui_end();
    ch8_free(machine);
}
