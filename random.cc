#include "adalyah.h"
#include <stdlib.h>
#include "random.h"

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
