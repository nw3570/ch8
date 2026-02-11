#ifndef CH8_DEFS_H
#define CH8_DEFS_H

/*
 * Largura padrão do display Chip-8 em pixels
 * 
 * Valor constante que define a resolução horizontal do display
 * conforme a especificação original do Chip-8
 */
#define CH8_DISPLAY_WIDTH 64u

/*
 * Altura padrão do display Chip-8 em pixels
 * 
 * Valor constante que define a resolução vertical do display
 * conforme a especificação original do Chip-8
 */
#define CH8_DISPLAY_HEIGHT 32u

/*
 * Valor representando um pixel ligado no display.
 * 
 * Utilizada para indicar que um pixel específico
 * deve ser exibido como visível.
 */
#define CH8_PIXEL_ON 1u

/*
 * Valor representando um pixel desligado no display.
 * 
 * Utilizada para indicar que um pixel específico deve
 * ser exibido como transparente/fundo.
 */
#define CH8_PIXEL_OFF 0u

/*
 * Número total de teclas no keypad do Chip-8
 * 
 * Define a quantidade de teclas hexadecimais disponíveis
 * na entrada do usuário (0x0 a 0xF)
 */
#define CH8_KEYS_COUNT 16u

/*
 * Declaração forward da estrutura principal do emulador.
 * 
 * Tipo opaco que representa a instância completa do emulador Chip-8.
 * A definição da estrutura está no módulo de implementação,
 * mantendo os detalhes internos encapsulados.
 */
typedef struct ch8 ch8_t;

#endif // CH8_DEFS_H