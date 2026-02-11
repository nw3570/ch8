#ifndef OPTS_H
#define OPTS_H

/*
 * Estrutura que armazena as opções de configuração do emulador.
 * 
 * Contém os parâmetros configuráveis fornecidos via linha de comando.
 * Todos os campos são inicializados com valores padrão e podem ser
 * modificados através dos argumentos passados ao programa.
 */
typedef struct {
    const char *program_path;  /* Caminho para o arquivo do programa Chip-8 a ser carregado */
    int cpu_hz;                /* Frequência de execução da CPU em Hertz (ciclos por segundo) */
} ch8opts_t;

/*
 * Processa os argumentos de linha de comando e popula a estrutura de opções.
 * 
 * Parâmetros:
 *  argc     Contador de argumentos (conforme passado para main()).
 *  argv     Vetor de strings com os argumentos (conforme passado para main()).
 *  options  Ponteiro para estrutura onde as opções parseadas serão armazenadas.
 * 
 * Retorno:
 *  0 em caso de sucesso, valor diferente de zero em caso de erro (argumentos 
 *  inválidos, falta de parâmetros obrigatórios, etc.).
 */
int ch8_parse_options(int argc, char **argv, ch8opts_t *options);

/*
 * Imprime mensagem de ajuda do programa, explicando o funcionamento.
 *
 * Parâmetros:
 *   prog_name Uma string com o nome do programa.
 */
void ch8_print_help(const char *prog_name);

#endif // OPTS_H
