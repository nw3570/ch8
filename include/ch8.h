#ifndef CH8_H
#define CH8_H

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t, uint16_t
#include "ch8_defs.h"

/*
 * Aloca e inicializa uma nova instância do emulador Chip-8.
 * 
 * Reserva memória para a estrutura principal do emulador e inicializa
 * todos os componentes internos (registradores, memória, display, etc.)
 * para seus valores de boot padrão.
 * 
 * Retorno: 
 *  Ponteiro para a instância alocada ou NULL em caso de falha.
 */
ch8_t *ch8_alloc();

/*
 * Libera todos os recursos associados a uma instância do emulador.
 * 
 * Desaloca a memória utilizada pela instância do emulador e por todos
 * os componentes internos associados a ela. Após esta chamada, o ponteiro
 * não deve mais ser utilizado.
 * 
 * Parâmetros:
 *  ch8 Ponteiro para a instância do emulador a ser liberada.
 */
void ch8_free(ch8_t *ch8);

/*
 * Carrega um programa Chip-8 a partir de um arquivo para a memória do emulador.
 * 
 * Lê o conteúdo do arquivo especificado e o copia para a área de memória
 * de programa do emulador, começando no endereço 0x200 (padrão Chip-8).
 * Valida o tamanho do programa para não exceder a capacidade de memória.
 * 
 * Parâmetros:
 *  ch8   Ponteiro para a instância do emulador.
 *  path  Caminho do arquivo contendo o programa Chip-8.
 * 
 * Retorno:
 *  Número de bytes carregados, ou 0 em caso de erro.
 */
size_t ch8_load_program(ch8_t *ch8, const char *path);

/*
 * Emula um ciclo de instrução da CPU Chip-8
 * 
 * Executa uma única instrução do programa atual: busca, decodifica e executa
 * a próxima instrução armazenada no contador de programa (PC). Atualiza
 * o estado interno do emulador conforme a operação realizada.
 * 
 * @param ch8 Ponteiro para a instância do emulador
 */
void ch8_emulate_cycle(ch8_t *ch8);

/*
 * Atualiza os timers de delay e som do Chip-8.
 * 
 * Decrementa os registradores de delay e sound (se maiores que zero)
 * em uma unidade. Deve ser chamada em intervalos regulares (tipicamente
 * 60Hz) para manter a temporização correta dos programas.
 * 
 * Parâmetros:
 *  ch8 Ponteiro para a instância do emulador.
 */
void ch8_tick_timers(ch8_t *ch8);

/*
 * Obtém o estado de um pixel específico do display.
 * 
 * Consulta o framebuffer interno para determinar se o pixel nas
 * coordenadas especificadas está ligado (ON) ou desligado (OFF).
 * 
 * Parâmetros:
 *  ch8 Ponteiro para a instância do emulador.
 *  x   Coordenada horizontal do pixel (0-63).
 *  y   Coordenada vertical do pixel (0-31).
 * 
 * Retorno:
 *  CH8_PIXEL_ON (1) se o pixel está ligado, CH8_PIXEL_OFF (0) se o pixel está
 *  desligado.
 */
uint8_t ch8_get_display_pixel(ch8_t *ch8, uint8_t x, uint8_t y);

/*
 * Verifica se o display precisa ser redesenhado.
 * 
 * Indica se houve alguma alteração no framebuffer desde a última
 * vez que a flag de redesenho foi limpa. Usado para otimização
 * gráfica, evitando redesenhar frames idênticos.
 * 
 * Parâmetros:
 *  ch8 Ponteiro para a instância do emulador.
 * 
 * Retorno:
 *  Diferente de zero se o display precisa ser redesenhado, zero caso contrário.
 */
int ch8_display_needs_redraw(ch8_t *ch8);

/*
 * Limpa a flag que indica necessidade de redesenho do display.
 * 
 * Reseta o estado interno que sinaliza que o display foi alterado.
 * Deve ser chamado após redesenhar a tela para permitir a detecção
 * de futuras modificações no framebuffer.
 * 
 * Parâmetros:
 *  ch8 Ponteiro para a instância do emulador.
 */
void ch8_display_clear_redraw(ch8_t *ch8);

/*
 * Atualiza o estado do teclado (keypad) do Chip-8.
 * 
 * Define o estado atual das 16 teclas hexadecimais do Chip-8.
 * Cada bit do parâmetro representa o estado de uma tecla
 * (1 = pressionada, 0 = liberada).
 * 
 * Parâmetros:
 *  ch8      Ponteiro para a instância do emulador.
 *  new_keys Máscara de 16 bits representando o estado das teclas,
 *           onde o bit 0 corresponde à tecla 0x0,
 *           bit 1 à tecla 0x1, ..., bit 15 à tecla 0xF.
 */
void ch8_set_keypad(ch8_t *ch8, uint16_t new_keys);

#endif // CH8_H