#include "ch8.h"
#include "timing.h"

void ch8_run(const char *program_path, int cpu_hz)
{
    // Inicializar componentes.
    uint8_t mem[CH8_MEMORY_SIZE] = { 0 };
    //uint8_t keypad[16] = { 0 };

    ch8cpu_t cpu = { 0 };
    cpu.program_counter = CH8_PROGRAM_START;

    ch8_load_font(mem);
    ch8_load_program(mem, program_path);

    ch8display_t display = { 0 };
    ch8_display_clear(&display);
    display.draw_flag = 0;

    // Timing
    double cpu_time_acc = 0.0;
    //double timers_time_acc = 0.0;
    double last_time = now_sec();

    while (1) {
        double current_time = now_sec();
        double delta = current_time - last_time;
        last_time = current_time;

        if (delta > 0.1)
            delta = 0.1;

        int cpu_ticks = count_ticks(&cpu_time_acc, 1.0 / cpu_hz, delta);
        for (int i = 0; i < cpu_ticks; i++) {
            ch8_step(&cpu, mem, &display);
        }

        ch8_display_ncurses_render(&display);
    }
}
