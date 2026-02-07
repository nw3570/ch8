#include "ch8.h"
#include <ncurses.h>

void ch8_step(ch8cpu_t *cpu, uint8_t *mem, ch8display_t *disp)
{
    uint16_t opcode = (mem[cpu->program_counter] << 8) | mem[cpu->program_counter + 1];
    cpu->program_counter += 2;

    uint8_t first_nibble = (opcode >> 12) & 0x0f;
    uint8_t kk = opcode & 0x00ff;
    uint16_t nnn = opcode & 0x0fff; 
    uint8_t n = opcode & 0x000f;
    uint8_t x = (opcode >> 8) & 0x0f;
    uint8_t y = (opcode >> 4) & 0x0f;

    switch (first_nibble) {
        case 0:
            switch (n) {
                case 0: // CLS
                    ch8_display_clear(disp);
                    break;
                case 0xe: // RET
                    break;   
            }
            break;
        case 0x1: // JP nnn
            cpu->program_counter = nnn;
            break;
        case 0x6: // LD Vx, kk
            cpu->V[x] = kk;
            break;
        case 0x7: // ADD Vx, kk
            cpu->V[x] += kk;
            break;
        case 0xA: // LD I, nnn
            cpu->index_register = nnn;
            break;
        case 0xD: // DRW Vx, Vy, n
            cpu->V[0xf] = ch8_display_draw(
                disp,
                cpu->V[x],
                cpu->V[y],
                n,
                &mem[cpu->index_register]
            );
            break;
        default:
            break;
    }
}
