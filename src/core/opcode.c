#include <stdlib.h> // rand()
#include "opcode.h"

void opcode_0xkk(ch8_t *ch8, uint8_t kk)
{
    switch (kk) {
        case 0xe0: // CLS
            ch8_display_clear(&ch8->disp);
            break;
        case 0xee: // RET
            subroutine_return(&ch8->cpu);
            break;
    }
}

void opcode_8xyn(uint8_t *V, uint8_t x, uint8_t y, uint8_t n)
{
    switch (n) {
        case 0x0: { // LD Vx, Vy
            V[x] = V[y];
            break;
        }
        case 0x1: { // OR Vx, Vy
            V[x] |= V[y];
            V[0xf] = 0;
            break;
        }
        case 0x2: { // AND Vx, Vy
            V[x] &= V[y];
            V[0xf] = 0;
            break;
        }
        case 0x3: { // XOR Vx, Vy
            V[x] ^= V[y];
            V[0xf] = 0;
            break;
        }
        case 0x4: { // ADD Vx, Vy
            uint16_t sum = V[x] + V[y];

            V[x] = (uint8_t) sum;
            V[0xf] = sum > 255; // Carry flag.
            break;
        }
        case 0x5: { // SUB Vx, Vy
            uint8_t borrow = V[y] > V[x];
            V[x] -= V[y];
            V[0xf] = !borrow;
            break;
        }
        case 0x6: { // SHR Vx {, Vy}
            uint8_t shifted_bit = V[y] & 0x01; // Bit menos significante.

            V[x] = V[y] >> 1;
            V[0xf] = shifted_bit;
            break;
        }
        case 0x7: { // SUBN Vx, Vy
            uint8_t borrow = V[x] > V[y];
            V[x] = V[y] - V[x];
            V[0xf] = !borrow; // NOT borrow flag.
            break;
        }
        case 0xe: { // SHL Vx {, Vy}
            uint8_t shifted_bit = (V[y] & 0x80) >> 7; // Bit mais significante.

            V[x] = V[y] << 1;
            V[0xf] = shifted_bit;
            break;
        }
    }
}

void opcode_fxkk(ch8_t *ch8, uint8_t x, uint8_t kk)
{
    switch (kk) {
        case 0x07: {
            // LD Vx, DT - Colocar o valor no registrador do delay timer em V[x].
            ch8->cpu.V[x] = ch8->delay_timer;
            break;
        }
        case 0x0a: {
            // LD Vx, K - Esperar por uma tecla ser pressionada e armazenar a
            // tecla em V[x].
            uint8_t pressed_key;

            if (ch8_key_first_pressed(ch8->keys, &pressed_key)) {
                ch8->cpu.V[x] = pressed_key;
            }
            else {
                ch8->cpu.pc -= 2;
            }

            break;
        }
        case 0x15: {
            // LD DT, Vx - Colocar o valor em V[x] no registrador do delay
            // timer.
            ch8->delay_timer = ch8->cpu.V[x];
            break;
        }
        case 0x18: {
            // LD ST, Vx - Colocar o valor em V[x] no registrador do sound
            // timer. Não implementado.
            break;
        }
        case 0x1e: {
            // ADD I, Vx - Adicionar no registrador I o valor em V[x].
            ch8->cpu.I += ch8->cpu.V[x];
            break;
        }
        case 0x29: {
            // LD F, Vx - Atribuir o endereço do caractere da fonte cujo valor é
            // V[x] no registrador I.
            ch8->cpu.I = CH8_FONT_START + ch8->cpu.V[x];
            break;
        }
        case 0x33: {
            // LD B, Vx - Carregar a representação BCD do valor em V[x] na
            // memória, nos endereços I para centena, I + 1 para dezena, I + 2
            // para unidade. 
            uint8_t value = ch8->cpu.V[x];

            ch8->mem[ch8->cpu.I] = value / 100; // Digito da centena.
            ch8->mem[ch8->cpu.I + 1] = (value / 10) % 10; // Digito da dezena.
            ch8->mem[ch8->cpu.I + 2] = value % 10; // Digito da unidade.
            break;
        }
        case 0x55: {
            // LD [I], Vx - Carrega os valores dos registradores na memória em
            // sequência, começando em V[0] e indo até V[x].
            for (uint8_t idx = 0; idx <= x; idx++) {
                ch8->mem[ch8->cpu.I + idx] = ch8->cpu.V[idx];
            }
            ch8->cpu.I += x + 1;
            break;
        }
        case 0x65: {
            // LD Vx, [I] - Carrega valores sequenciais na memória para os
            // registradores, começando em V[0] e indo até V[x].
            for (uint8_t idx = 0; idx <= x; idx++) {
                ch8->cpu.V[idx] = ch8->mem[ch8->cpu.I + idx];
            }
            ch8->cpu.I += x + 1;
            break;
        }
    }
}

void opcode_exkk(ch8cpu_t *cpu, uint16_t keys, uint8_t x, uint8_t kk)
{
    int condition = ch8_key_is_pressed(keys, cpu->V[x]);

    if (kk == 0xa1) {
        condition = !condition;
    }

    if (condition) {
        cpu->pc += 2;   
    }
}

int subroutine_call(ch8cpu_t *cpu, uint16_t sub_addr)
{
    if (cpu->sp >= CH8_STACK_DEPTH)
        return 0;

    cpu->call_stack[cpu->sp++] = cpu->pc;
    cpu->pc = sub_addr;

    return 1;
}

int subroutine_return(ch8cpu_t *cpu)
{
    if (cpu->sp == 0)
        return 0;

    cpu->pc = cpu->call_stack[--cpu->sp];
    return 1;
}

uint8_t random_byte(uint8_t kk)
{
    return (uint8_t) rand() & kk;
}
