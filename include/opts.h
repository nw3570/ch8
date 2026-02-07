#ifndef OPTS_H
#define OPTS_H

typedef struct {
    const char *program_path;
    int cpu_hz;
} ch8opts_t;

int ch8_parse_options(int argc, char **argv, ch8opts_t *options);

#endif // OPTS_H
