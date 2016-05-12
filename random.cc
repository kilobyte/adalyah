#include "adalyah.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "random.h"

void seed_rng()
{
    uint32_t seed;

    int fd = open("/dev/urandom", O_RDONLY);
    if (fd >= 0)
    {
        read(fd, &seed, sizeof(seed));
        close(fd);
    }

    seed ^= getpid()+time(0);
    srand(seed);
}

int rnd(int max)
{
    if (max <= 1)
        return 0;
    assert(max <= RAND_MAX);

    uint32_t partn = RAND_MAX / max;

    while (1)
    {
        uint32_t val = rand() / partn;

        if (val < (uint32_t)max)
            return val;
    }
}
