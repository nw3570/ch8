#include "emulator.h"
#include "opts.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

int main(int argc, char *argv[])
{
    ch8opts_t options;

    int status = ch8_parse_options(argc, argv, &options);

    if (status == 1)
        printf("help");

    if (status > 0)
        exit(status);

    emulator_run(options.program_path, options.cpu_hz);
    return 0;
}
