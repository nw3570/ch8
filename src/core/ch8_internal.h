#ifndef CH8_INTERNAL
#define CH8_INTERNAL

#include "ch8_defs.h"
#include <stdint.h> // uint8_t, uint16_t
#include <stddef.h> // size_t

//
#define CH8_MEMORY_SIZE 4096u
#define CH8_PROGRAM_START 0x200u
#define CH8_FONT_START 0x05u
#define CH8_REGISTERS 16u
#define CH8_STACK_DEPTH 16u

//
typedef struct {
    uint8_t draw_flag;
    uint8_t pixels[CH8_DISPLAY_HEIGHT][CH8_DISPLAY_WIDTH];
} ch8display_t;

typedef struct {
    uint8_t V[CH8_REGISTERS];
    uint8_t sp;
    uint16_t call_stack[CH8_STACK_DEPTH];
    uint16_t pc;
    uint16_t I;
} ch8cpu_t;

// 
struct ch8 { 
    uint8_t delay_timer;
    //, sound_timer;
    uint8_t mem[CH8_MEMORY_SIZE];
    ch8display_t disp;
    ch8cpu_t cpu;
    //uint16_t keys;
};

// Display
void ch8_display_init(ch8display_t *);
void ch8_display_clear(ch8display_t *);
int  ch8_display_draw(
    ch8display_t *,
    uint8_t x,
    uint8_t y,
    uint8_t n,
    const uint8_t *data
);

// Memória
void ch8_load_font(uint8_t *mem);

// Teclado
uint8_t ch8_key_is_pressed(uint16_t keys, uint8_t target);
uint8_t ch8_key_any_pressed(uint16_t keys);

// CPU
void     ch8_cpu_init(ch8cpu_t *);
uint16_t ch8_fetch_instruction(ch8cpu_t *, const uint8_t *mem);
void     ch8_execute_instruction(ch8_t *, uint16_t opcode);

#endif // CH8_INTERNAL
