#ifndef UI_H
#define UI_H

#include "ch8_defs.h" // ch8_t
#include <stdint.h> // uint8_t

#define UI_KEYS_FRAME_TIMEOUT 15

typedef struct {
    uint8_t pressed;
    uint8_t frames_left;
} uikey_t;

/*
 * Inicializa o subsistema de interface de usuário
 * 
 * Configura o ambiente de UI, preparando recursos necessários para
 * renderização e entrada de usuário. Deve ser chamada antes de qualquer
 * outra função deste módulo.
 */
void ui_setup();

/*
 * Finaliza o subsistema de interface de usuário
 * 
 * Libera todos os recursos alocados pelo módulo de UI e restaura
 * o estado original do terminal. Deve ser chamada antes do término
 * do programa.
 */
void ui_end();

/*
 * Renderiza o estado atual do display Chip-8 na interface.
 * 
 * Converte o framebuffer interno do emulador para uma representação
 * visual na tela, respeitando as dimensões padrão do Chip-8 (64x32).
 * 
 * Parâmetros:
 *  ch8 Ponteiro para a instância do emulador cujo display será renderizado.
 */
void ui_display_render(ch8_t *);


void ui_keys_update(uikey_t keys[CH8_KEYS_COUNT]);

#endif // UI_H
