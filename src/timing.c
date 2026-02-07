#include <time.h>

double now_sec()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec + ts.tv_nsec * 1e9;
}

int count_ticks(double *acc, double period, double delta)
{
    if (acc)
        *acc += delta;

    int ticks = 0;
    while (*acc >= period) {
        *acc -= period;
        ticks++;
    }

    return ticks;
}
