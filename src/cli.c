#include "cli.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#define OPTS_SHORT "+f:h"

int ch8_parse_options(int argc, char **argv, ch8opts_t *options)
{
    if (options == NULL)
        return 0;

    options->cpu_hz = 700;
    int opt;

    while ((opt = getopt(argc, argv, OPTS_SHORT)) != -1) {
        switch (opt) {
            case 'h': // help
                return 0;
            case 'f': // Frequência da CPU
                options->cpu_hz = atoi(optarg);
                break;
            default:
                break;
        }
    }

    if (optind >= argc)
        return 1;
    
    options->program_path = argv[optind];

    return 0;
}


void ch8_print_help(const char *prog_name)
{
    printf("CH8 - Emulador CHIP-8\n\n");
    printf("Uso:\n");
    printf("  %s [opções] <arquivo>\n\n", prog_name);

    printf("Opções:\n");
    printf("  -f <hz>    Frequência da CPU (padrão: 700 Hz)\n");
    printf("  -h         Exibe esta ajuda\n\n");

    printf("Mapeamento de Teclado:\n\n");
    printf("  CHIP-8        Teclado PC\n");
    printf("  1 2 3 C       1 2 3 4\n");
    printf("  4 5 6 D  ->   Q W E R\n");
    printf("  7 8 9 E       A S D F\n");
    printf("  A 0 B F       Z X C V\n");
}
