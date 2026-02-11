#ifndef OPCODE_H
#define OPCODE_H

#include "ch8_internal.h"

int subroutine_call(ch8cpu_t *cpu, uint16_t sub_addr);
int subroutine_return(ch8cpu_t *cpu);
uint8_t random_byte(uint8_t kk);

void opcode_0xkk(ch8_t *ch8, uint8_t kk);
void opcode_8xyn(uint8_t *V, uint8_t x, uint8_t y, uint8_t n);
void opcode_exkk(ch8cpu_t *cpu, uint16_t keys, uint8_t x, uint8_t kk);
void opcode_fxkk(ch8_t *ch8, uint8_t x, uint8_t kk);

#endif // OPCODE_H
