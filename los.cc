#include "adalyah.h"
#include "los.h"
#include "map.h"

#include "term.h"

bool vision(coord a, coord b)
{
    if (a == b)
        return true;

    int dist = (b - a).len();
    if (dist > 8)
        return false;
    int ang0 = (b - a).dir() * 60;
    for (int angd = 0; angd <= 60; ++angd)
    {
        angle360_iterator ani(ang0 + angd, a);
        for (int d = 1; d < dist; ++d)
        {
            ++ani;
            if (fmap(*ani) != FEAT_FLOOR)
                goto blocked;
        }
        ++ani; // the last spot is not checked for blockedness
        if (*ani == b)
            return true;
blocked:;
    }

    return false;
}
