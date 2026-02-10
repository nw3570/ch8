#include <string.h> // memset
#include "ch8_internal.h"

struct decode {
    uint8_t a, x, y, n;
    uint8_t kk;
    uint16_t nnn;
};

void ch8_cpu_init(ch8cpu_t *cpu)
{
    if (!cpu)
        return;

    cpu->pc = 0;
    cpu->I  = 0;
    cpu->sp = 0;

    memset(&cpu->call_stack, 0, sizeof (uint16_t) * 3);
    memset(&cpu->V, 0, sizeof (uint8_t) * 16);
}

uint16_t ch8_fetch_instruction(ch8cpu_t *cpu, const uint8_t *mem)
{
    uint16_t opcode = mem[cpu->pc] << 8;
    opcode = opcode | mem[cpu->pc + 1];

    return opcode;
}

static void __opcode_decode(uint16_t opcode, struct decode *decode)
{
    decode->a = (opcode >> 12) & 0x0f;
    decode->x = (opcode >> 8 ) & 0x0f;
    decode->y = (opcode >> 4 ) & 0x0f;
    decode->n = opcode & 0x000f;

    decode->kk  = opcode & 0x00ff;
    decode->nnn = opcode & 0x0fff;
}

void ch8_execute_instruction(ch8_t *ch8, uint16_t opcode)
{
    ch8->cpu.pc += 2;

    struct decode dec;
    __opcode_decode(opcode, &dec);

    switch (dec.a) {
        case 0x0:
            if (dec.kk == 0xe0) // CLS
                ch8_display_clear(&ch8->disp);
            break;
        case 0x1: // JP nnn
            ch8->cpu.pc = dec.nnn;
            break;
        case 0x6: // LD Vx, kk
            ch8->cpu.V[dec.x] = dec.kk;
            break;
        case 0x7: // ADD Vx, kk
            ch8->cpu.V[dec.x] += dec.kk;
            break;
        case 0xA: // LD I, nnn
            ch8->cpu.I = dec.nnn;
            break;
        case 0xD: // DRW Vx, Vy, n
            ch8->cpu.V[0xf] = ch8_display_draw(
                &ch8->disp,
                ch8->cpu.V[dec.x],
                ch8->cpu.V[dec.y],
                dec.n,
                &ch8->mem[ch8->cpu.I]
            );
            break;
    }
}
