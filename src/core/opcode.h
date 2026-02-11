#ifndef OPCODE_H
#define OPCODE_H

#include "ch8_internal.h"

/*
 * Executa uma chamada de subrotina (instrução CALL).
 * 
 * Salva o endereço de retorno atual na pilha e atualiza o
 * Program Counter para iniciar a execução da subrotina.
 * Respeita a profundidade máxima da pilha definida por
 * CH8_STACK_DEPTH.
 * 
 * Parâmetros:
 *  cpu     Ponteiro para a estrutura da CPU
 *  sub_addr Endereço da subrotina a ser chamada
 * 
 * Retorno:
 *  1 em caso de sucesso, 0 se a pilha estiver cheia.
 */
int subroutine_call(ch8cpu_t *cpu, uint16_t sub_addr);

/*
 * Retorna de uma subrotina (instrução RET).
 * 
 * Restaura o contador de programa a partir do topo da pilha,
 * retomando a execução no endereço seguinte à chamada original.
 * 
 * Parâmetros:
 *  cpu Ponteiro para a estrutura da CPU
 * 
 * Retorno:
 *  1 em caso de sucesso, 0 se a pilha estiver vazia.
 */
int subroutine_return(ch8cpu_t *cpu);

/*
 * Gera um byte pseudoaleatório no intervalo [0, 255].
 * 
 * Implementa a geração de números aleatórios necessária para
 * a instrução RND Vx, kk.
 * 
 * @param kk Valor que usado como máscara em cima do número aleatóro gerado.
 * 
 * Retorno:
 *  Byte aleatório no intervalo completo de 0 a 255
 */
uint8_t random_byte(uint8_t kk);

/*
 * Processa instruções da família 0xkkk (instruções diversas).
 * 
 * Trata opcodes que começam com 0, incluindo:
 * - 0x00E0: Limpar tela.
 * - 0x00EE: Retornar de subrotina.
 * - 0x0NNN: Chamada para máquina (legado, não implementado).
 * 
 * Parâmetros:
 *  ch8 Ponteiro para a instância completa do emulador.
 *  kk  Byte menos significativo do opcode (para instruções 0x0NNN).
 */
void opcode_0xkk(ch8_t *ch8, uint8_t kk);

/*
 * Processa instruções da família 0x8xyn (operações aritméticas/lógicas)
 * 
 * Executa operações entre registradores Vx e Vy baseadas no nibble 'n':
 * - 0x0: Vx = Vy        - atribuição entre registradores.
 * - 0x1: Vx = Vx OR Vy  - "ou" lógico bit-a-bit.
 * - 0x2: Vx = Vx AND Vy - "e" lógico bit-a-bit.
 * - 0x3: Vx = Vx XOR Vy - "ou exclusivo" lógico bit-a-bit.
 * - 0x4: Vx = Vx + Vy   - soma de 8 bits com carry.
 * - 0x5: Vx = Vx - Vy   - subtração de 8 bits com borrow.
 * - 0x6: Vx = Vx >> 1   - shift para esquerda.
 * - 0x7: Vx = Vy - Vx   - subtração de 8 bitscom borrow, ordem inversa de operandos.
 * - 0xE: Vx = Vx << 1   - shift para direita.
 * 
 * Parâmetros:
 *  V  Array de registradores V0-VF.
 *  x  Índice do registrador destino (0x0-0xF).
 *  y  Índice do registrador fonte (0x0-0xF).
 *  n  Nibble que especifica a operação (0x0-0xE).
 */
void opcode_8xyn(uint8_t *V, uint8_t x, uint8_t y, uint8_t n);

/*
 * Processa instruções da família 0xExkk (condicionais de teclado).
 * 
 * Executa saltos condicionais baseados no estado do teclado:
 * - 0xEx9E: Pula próxima instrução se tecla Vx pressionada.
 * - 0xExA1: Pula próxima instrução se tecla Vx NÃO pressionada.
 * 
 * Parâmetros:
 *  cpu  Ponteiro para a estrutura da CPU.
 *  keys Máscara de 16 bits com estado atual do teclado.
 *  x    Índice do registrador que contém o código da tecla.
 *  kk   Byte menos significativo do opcode (0x9E ou 0xA1).
 */
void opcode_exkk(ch8cpu_t *cpu, uint16_t keys, uint8_t x, uint8_t kk);

void opcode_exkk(ch8cpu_t *cpu, uint16_t keys, uint8_t x, uint8_t kk);

/*
 * Processa instruções da família 0xFxkk (operações diversas com registrador).
 * 
 * Executa várias operações usando o registrador Vx:
 * - 0x07: Vx = delay_timer.
 * - 0x0A: Aguarda tecla e armazena em Vx.
 * - 0x15: delay_timer = Vx.
 * - 0x18: sound_timer = Vx.
 * - 0x1E: I = I + Vx.
 * - 0x29: I = endereço do sprite do caractere em Vx.
 * - 0x33: Armazena representação BCD de Vx em [I], [I+1], [I+2].
 * - 0x55: Salva registradores V0-Vx na memória a partir de [I].
 * - 0x65: Carrega registradores V0-Vx da memória a partir de [I].
 * 
 * Parâmetros:
 *  ch8 Ponteiro para a instância completa do emulador.
 *  x   Índice do registrador Vx (0x0-0xF).
 *  kk  Byte menos significativo que especifica a operação.
 */
void opcode_fxkk(ch8_t *ch8, uint8_t x, uint8_t kk);

#endif // OPCODE_H
