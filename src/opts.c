#include "opts.h"
#include <stdlib.h>
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
                return 1;
            case 'f': // Frequência da CPU
                options->cpu_hz = atoi(optarg);
                break;
            default:
                break;
        }
    }

    if (optind >= argc)
        return 2;
    
    options->program_path = argv[optind];

    return 0;
}
