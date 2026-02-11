#ifndef CH8_INTERNAL_H
#define CH8_INTERNAL_H

#include "ch8_defs.h"
#include <stdint.h> // uint8_t, uint16_t
#include <stddef.h> // size_t

/*
 * Tamanho total da memória do Chip-8 em bytes.
 * 
 * Define a capacidade total do espaço de endereçamento do Chip-8,
 * que inclui memória de programa, dados, fontes e área do sistema.
 */
#define CH8_MEMORY_SIZE 4096u

/*
 * Endereço inicial padrão para carregamento de programas
 * 
 * A maioria dos programas Chip-8 começa neste endereço (0x200),
 * evitando a área reservada para o interpretador e fontes.
 */
#define CH8_PROGRAM_START 0x200u

/*
 * Endereço inicial da tabela de fontes na memória.
 * 
 * Local onde os sprites dos caracteres hexadecimais (0-F) são
 * armazenados na memória para uso pela instrução DRW.
 */
#define CH8_FONT_START 0x05u

/*
 * Número de registradores de propósito geral (V0-VF)
 * 
 * Define a quantidade de registradores de 8 bits disponíveis
 * para operações do Chip-8.
 */
#define CH8_REGISTERS 16u

/*
 * Profundidade máxima da pilha de chamadas
 * 
 * Limita o número máximo de chamadas aninhadas suportadas
 * pela instrução CALL.
 */
#define CH8_STACK_DEPTH 16u

/*
 * Estrutura que representa o display gráfico do Chip-8.
 * 
 * Mantém o estado completo do framebuffer monocromático 64x32
 * e uma flag que indica quando o conteúdo visual foi alterado.
 */
typedef struct {
    uint8_t draw_flag; // Flag que indica se o display foi modificado desde o último redesenho.
    uint8_t pixels[CH8_DISPLAY_HEIGHT][CH8_DISPLAY_WIDTH];  // Matriz do framebuffer.
} ch8display_t;

/*
 * Estrutura que representa o estado da CPU do Chip-8
 * 
 * Contém todos os registradores e componentes de estado
 * necessários para a execução de instruções.
 */
typedef struct {
    uint8_t V[CH8_REGISTERS];             // Registradores de propósito geral V0 a VF.
    uint8_t sp;                           // Stack Pointer - aponta para o topo da pilha.
    uint16_t call_stack[CH8_STACK_DEPTH]; // Pilha de endereços de retorno para CALL/RET.
    uint16_t pc;                          // Contador de programa, armazena o endereço da próxima instrução.
    uint16_t I;                           // Registrador de índice, usado para endereçamento de memória.
} ch8cpu_t;

/*
 * Estrutura principal que representa uma instância completa do emulador Chip-8
 * 
 * Agrega todos os subsistemas (memória, CPU, display) em uma única entidade.
 * Esta é a definição completa do tipo opaco ch8_t declarado em ch8_defs.h.
 */
struct ch8 { 
    uint8_t delay_timer;          // Timer de delay - decrementado a 60Hz quando > 0.
    //uint8_t sound_timer;        // Timer de som - não implementado.
    uint8_t mem[CH8_MEMORY_SIZE]; // Memória principal do sistema.
    ch8display_t disp;            // Estado do display gráfico.
    ch8cpu_t cpu;                 // Estado da CPU e seus registradores.
    uint16_t keys;                // Estado atual do keypad - cada bit representa uma tecla (0-15).
};

/* ==================== SUBSISTEMA DE DISPLAY ==================== */

/*
 * Inicializa uma estrutura de display com valores padrão
 * 
 * Define todos os pixels como desligados e reseta a flag de redesenho.
 * 
 * Parâmetros:
 *  disp Ponteiro para a estrutura de display a ser inicializada
 */
void ch8_display_init(ch8display_t *);

/*
 * Limpa completamente o display.
 * 
 * Define todos os pixels como desligados (CH8_PIXEL_OFF) e
 * ativa a flag de redesenho para indicar que a tela mudou.
 * 
 * Parâmetros:
 *  disp Ponteiro para a estrutura de display a ser limpa
 */
void ch8_display_clear(ch8display_t *);

/*
 * Desenha um sprite no display.
 * 
 * Implementa a instrução DRW do Chip-8: copia um sprite de n bytes
 * para as coordenadas (x,y), usando operação XOR. Se algum pixel
 * for desligado como resultado, define VF = 1 (flag de colisão).
 * 
 * Parâmetros:
 *  disp Ponteiro para a estrutura de display de destino
 *  x    Coordenada horizontal do sprite.
 *  y    Coordenada vertical do sprite.
 *  n    Altura do sprite em pixels (número de bytes).
 *  data Ponteiro para os dados do sprite (n bytes).
 * 
 * Retorno:
 *  1 se houve colisão (pixel desligado), 0 caso contrário.
 */
int  ch8_display_draw(
    ch8display_t *,
    uint8_t x,
    uint8_t y,
    uint8_t n,
    const uint8_t *data
);

/* ==================== SUBSISTEMA DE MEMÓRIA ==================== */

/*
 * Carrega o conjunto padrão de fontes hexadecimais na memória.
 * 
 * Copia os sprites dos caracteres 0-F para a área de fontes
 * da memória, começando em CH8_FONT_START.
 * 
 * Parâmetros:
 *  mem Ponteiro para o array de memória onde as fontes serão carregadas.
 */
void ch8_load_font(uint8_t *mem);

/* ==================== SUBSISTEMA DE TECLADO ==================== */

/*
 * Verifica se uma tecla específica está pressionada
 * 
 * Testa o bit correspondente à tecla alvo na máscara de teclas atual.
 * 
 * Parâmetros:
 *  keys   Máscara de 16 bits representando o estado das teclas.
 *  target Índice da tecla a verificar (0x0 a 0xF).
 * 
 * Retorno:
 *  1 se a tecla está pressionada, 0 caso contrário.
 */
int ch8_key_is_pressed(uint16_t keys, uint8_t target);

/*
 * Encontra a primeira tecla pressionada no estado do teclado.
 * 
 * Varre a máscara de teclas da esquerda para a direita (0 a F)
 * até encontrar uma tecla pressionada. Útil para implementação
 * da instrução de espera por tecla (LD Vx, K).
 * 
 * Parâmetros:
 *  keys    Máscara de 16 bits representando o estado das teclas.
 *  out_key Ponteiro onde será armazenado o índice da tecla encontrada.
 * 
 * Retorno:
 *  1 se encontrou uma tecla pressionada, 0 se nenhuma tecla está pressionada.
 */
int ch8_key_first_pressed(uint16_t keys, uint8_t *out_key);

/* ==================== SUBSISTEMA DA CPU ==================== */

/*
 * Inicializa uma estrutura de CPU.
 * 
 * Define todos os registradores como zero, PC como CH8_PROGRAM_START,
 * e reseta o stack pointer. Prepara a CPU para executar um novo programa.
 *
 * Parâmetros: 
 *  cpu Ponteiro para a estrutura de CPU a ser inicializada.
 */
void ch8_cpu_init(ch8cpu_t *);

/*
 * Busca a próxima instrução na memória
 * 
 * Lê dois bytes a partir do endereço apontado pelo contador de programa.
 * 
 * Parâmetros:
 *  cpu Ponteiro para a estrutura de CPU com o PC atual
 *  mem Ponteiro para o array de memória principal.
 *
 * Retorno:
 *  O opcode de 16 bits da próxima instrução a executar.
 */
uint16_t ch8_fetch_instruction(ch8cpu_t *, const uint8_t *mem);

/*
 * Decodifica e executa uma instrução do Chip-8
 * 
 * Interpreta o opcode fornecido, realizando a operação correspondente
 * sobre o estado do emulador.
 * 
 * Parâmetros:
 *  ch8    Ponteiro para a instância completa do emulador.
 *  opcode Instrução de 16 bits a ser executada.
 */
void ch8_execute_instruction(ch8_t *, uint16_t opcode);

#endif // CH8_INTERNAL_H