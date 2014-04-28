#include "adalyah.h"
#include <stdlib.h>
#include "coord.h"

coord Compass[6] =
{
    coord( 1, 0),
    coord( 0,-1),
    coord(-1,-1),
    coord(-1, 0),
    coord( 0, 1),
    coord( 1, 1),
};

static int Rotations[6][4] =
{
    { 1, 0,  0, 1 },
    { 0,-1,  1, 1 },
    {-1,-1,  1, 0 },
    {-1, 0,  0,-1 },
    { 0, 1, -1,-1 },
    { 1, 1, -1, 0 },
};

coord coord::rotate(int d) const // ↺
{
    d = (d + 600000000) % 6;
    return coord(x * Rotations[d][0] + y * Rotations[d][2],
                 x * Rotations[d][1] + y * Rotations[d][3]);
}

int coord::dir() const
{
    if (y == 0)
        return x >= 0 ? 0 : 3;
    if (y < 0)
        if (x > 0)
            return 0;
        else if (x > y)
            return 1;
        else
            return 2;
    else
        if (x < 0)
            return 3;
        else if (x < y)
            return 4;
        else
            return 5;
}

void test_coord()
{
    assert(coord(1,2) == coord(1,2));
    assert(coord(1,2) != coord(1,0));
    assert(coord(1,2) != coord(2,2));

    assert(coord(0,2) + coord(1,3) - coord(2,4) == coord(-1,1));

    assert(coord(0,0).len() == 0);
    for (int i = 0; i < 6; i++)
        assert(Compass[i].len() == 1);
    assert(coord(0,2).len() == 2);
    assert(coord(1,-1).len() == 2);

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
            assert(Compass[i].rotate(j - i) == Compass[j]);

    assert(coord(0,0).dir() == 0);
    for (int i = 0; i < 6; i++)
        assert(Compass[i].dir() == i);
    for (int x = -2; x <= +2; x++)
        for (int y = -2; y <= +2; y++)
            if (x || y)
            {
                int dir = coord(x,y).dir();
                assert((dir + 3) % 6 == coord(-x,-y).dir());
                for (int i = 0; i < 6; i++)
                    assert(coord(x,y).rotate(i).dir() == (dir + i) % 6);
            }
}
