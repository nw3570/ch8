#include "emulator.h"
#include "cli.h"

int main(int argc, char *argv[])
{
    ch8opts_t options;

    int status = ch8_parse_options(argc, argv, &options);

    if (status != 0) {
        ch8_print_help(argv[0]);
        return status;
    }

    emulator_run(options.program_path, options.cpu_hz);
    return 0;
}
