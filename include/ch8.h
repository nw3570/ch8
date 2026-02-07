#ifndef CH8_H
#define CH8_H

#include <stdint.h>
#include <stddef.h>

// Memory
#define CH8_MEMORY_SIZE 4096
#define CH8_PROGRAM_START 0X200
#define CH8_FONT_START 0x50

size_t ch8_load_program(uint8_t *mem, const char *path);
void ch8_load_font(uint8_t *mem);

// Display
#define CH8_DISPLAY_WIDTH 64
#define CH8_DISPLAY_HEIGHT 32
#define CH8_DISPLAY_PIXEL_ON 0xFF
#define CH8_DISPLAY_PIXEL_OFF 0x00
#define CH8_DISPLAY_FPS 60

typedef struct ch8display ch8display_t;

struct ch8display {
    uint8_t draw_flag;
    uint8_t pixels[CH8_DISPLAY_HEIGHT][CH8_DISPLAY_WIDTH];
};


void ch8_display_clear(ch8display_t *);
int ch8_display_draw(
    ch8display_t *,
    uint8_t x,
    uint8_t y,
    uint8_t n,
    const uint8_t *sprite_rows
);
void ch8_display_ncurses_render(ch8display_t *disp);
// Keypad
#define CH8_KEYPAD_SIZE 16

// int  ch8_key_is_pressed(uint8_t *keypad, uint8_t key);
// void ch8_key_wait(uint8_t *keypad, uint8_t key);

// CPU
#define CH8_CALL_STACK_SIZE 3
#define CH8_VREGISTERS_NO 16
#define CH8_CPU_DEFAULT_FREQ_HZ 700

typedef struct ch8cpu ch8cpu_t;

struct ch8cpu {
    uint8_t V[CH8_VREGISTERS_NO];
    uint16_t index_register;
    uint16_t program_counter;
};

void ch8_step(ch8cpu_t *cpu, uint8_t *mem, ch8display_t *disp);
void ch8_run(const char *program_path, int cpu_hz);
#endif // CH8_H
