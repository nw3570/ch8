#include <string.h> // memset
#include <stdlib.h> // rand()
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

static void __opcode_0xxx(ch8_t *ch8, struct decode *dec)
{
    switch (dec->kk) {
        case 0xe0: // CLS
            ch8_display_clear(&ch8->disp);
            break;
        case 0xee: // RET
            ch8->cpu.sp--;
            uint16_t ret_addr = ch8->cpu.call_stack[ch8->cpu.sp];

            ch8->cpu.pc = ret_addr;
            break;
    }
}

static void __opcode_8xxx(uint8_t *V, struct decode *dec)
{
    switch (dec->n) {
        case 0x0: // LD Vx, Vy
            V[dec->x] = V[dec->y];
            break;
        case 0x1: // OR Vx, Vy
            V[dec->x] |= V[dec->y];
            break;
        case 0x2: // AND Vx, Vy
            V[dec->x] &= V[dec->y];
            break;
        case 0x3: // XOR Vx, Vy
            V[dec->x] ^= V[dec->y];
            break;
        case 0x4: { // ADD Vx, Vy
            uint16_t sum = V[dec->x] + V[dec->y];
            
            V[0xf] = sum > 255;
            V[dec->x] = (uint8_t) sum;
            break;
        }
        case 0x5: // SUB
            V[0xf] = V[dec->x] > V[dec->y];
            V[dec->x] -= V[dec->y];
            break;
        case 0x6: // SHR
            V[dec->x] = V[dec->y] >> 1;
            break;
        case 0x7: // SUBN
            V[0xf] = V[dec->y] > V[dec->x];
            V[dec->x] = V[dec->y] - V[dec->x];
            break;
        case 0xe: // SHL
            V[dec->x] = V[dec->y] << 1;
            break;
    }
}

static void __opcode_fxxx(ch8_t *ch8, struct decode *dec)
{
    switch (dec->kk) {
        case 0x07: // LD Vx, DT - Pegar valor timer de delay.
            ch8->cpu.V[dec->x] = ch8->delay_timer;
            break;
        case 0x15: // LD DT, Vx - Settar timer de delay.
            ch8->delay_timer = ch8->cpu.V[dec->x];
            break;
        case 0x18: // LD ST, Vx - Settar timer de som.
            // Não implementado!
            break;
        case 0x0a: // LD Vx, K
            break;
        case 0x1e: // ADD I, Vx
            ch8->cpu.I += ch8->cpu.V[dec->x];
            break;
        case 0x29:
            ch8->cpu.I = CH8_FONT_START + ch8->cpu.V[dec->x];
            break;
        case 0x33: {
            uint8_t value = ch8->cpu.V[dec->x];

            ch8->mem[ch8->cpu.I] = value / 100;
            ch8->mem[ch8->cpu.I + 1] = (value / 10) % 10;
            ch8->mem[ch8->cpu.I + 2] = value % 10;
            break;
        }
        case 0x55: // LD [I], Vx
            for (uint8_t idx = 0; idx < dec->x; idx++) {
                ch8->mem[ch8->cpu.I + idx] = ch8->cpu.V[idx];
            }
            break;
        case 0x65: // LD Vx, [I]
            for (uint8_t idx = 0; idx < dec->x; idx++) {
                ch8->cpu.V[idx] = ch8->mem[ch8->cpu.I + idx];
            }
            break;
    }
}

void ch8_execute_instruction(ch8_t *ch8, uint16_t opcode)
{
    ch8->cpu.pc += 2;

    struct decode dec;
    __opcode_decode(opcode, &dec);

    switch (dec.a) {
        case 0x0:
            __opcode_0xxx(ch8, &dec);
            break;
        case 0x1: // JP nnn
            ch8->cpu.pc = dec.nnn;
            break;
        case 0x2: // CALL nnn
            ch8->cpu.call_stack[ch8->cpu.sp] = ch8->cpu.pc;
            ch8->cpu.sp++;

            ch8->cpu.pc = dec.nnn;
            break;
        case 0x3: // SE Vx, kk
            if (ch8->cpu.V[dec.x] == dec.kk)
                ch8->cpu.pc += 2;
            break;
        case 0x4:
            if (ch8->cpu.V[dec.x] != dec.kk)
                ch8->cpu.pc += 2;
            break;
        case 0x5:
            if (ch8->cpu.V[dec.x] == ch8->cpu.V[dec.y])
                ch8->cpu.pc += 2;
            break;
        case 0x6: // LD Vx, kk
            ch8->cpu.V[dec.x] = dec.kk;
            break;
        case 0x7: // ADD Vx, kk
            ch8->cpu.V[dec.x] += dec.kk;
            break;
        case 0x8:
            __opcode_8xxx(ch8->cpu.V, &dec);
            break;
        case 0x9:
            if (ch8->cpu.V[dec.x] != ch8->cpu.V[dec.y])
                ch8->cpu.pc += 2;
            break;
        case 0xa: // LD I, nnn
            ch8->cpu.I = dec.nnn;
            break;
        case 0xb: // JP V0, nnn
            ch8->cpu.pc = ch8->cpu.V[0] + dec.nnn;
            break;
        case 0xc: {
            uint8_t random_byte = rand() % 255;
            ch8->cpu.V[dec.x] = random_byte & dec.kk;
            break;
        }
        case 0xd: // DRW Vx, Vy, n
            ch8->cpu.V[0xf] = ch8_display_draw(
                &ch8->disp,
                ch8->cpu.V[dec.x],
                ch8->cpu.V[dec.y],
                dec.n,
                &ch8->mem[ch8->cpu.I]
            );
            break;
        case 0xe:
            break;
        case 0xf:
            __opcode_fxxx(ch8, &dec);
            break;
    }
}
