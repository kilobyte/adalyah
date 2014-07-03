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
    coord vect = b - a;
    int dir = vect.dir();
    coord v[2];
    v[0] = Compass[dir];
    v[1] = Compass[(dir+1)%6];
    vect = vect.rotate(-dir); // vect.x is positive, vect.y is negative
    if (!vect.y)
        v[1]=v[0], vect.x=1; // degenerate case
    for (int d0 = vect.y; d0 < vect.x; ++d0)
    {
        coord pos = a;
        int d = d0;
        for (int i = 1; i < dist; ++i)
        {
            if (d >= 0)
                pos += v[0], d += vect.y;
            else
                pos += v[1], d += vect.x;
            if (fmap(pos) != FEAT_FLOOR)
                goto blocked;
        }
        return true;
    blocked: ;
    }
    return false;
}


void test_vision(void)
{
    for (spiral_iterator a(coord(0,0), 20, true); a; ++a)
    {
        assert(vision(*a, *a));

        for (spiral_iterator b(*a, 8); b; ++b)
        {
            if (*a > *b)
                continue;
            assert(vision(*a, *b) == vision(*b, *a));
        }
    }
}
