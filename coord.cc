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

// This is only an approximation.  Even in theory, vectors longer than 60
// can't be represented uniquely -- and here, the first hex that
// angle360_iterator for its angle360 doesn't visit is at distance 34.
int coord::angle360() const
{
    int d = dir();
    coord s = rotate(-d);
    if (!s.y)
        return d * 60;
    return d * 60 - s.y * 60 / (s.x - s.y);
}

angle360_iterator::angle360_iterator(int _angle, coord start)
    : cur(start), err(0)
{
    angle = (_angle + 900000000) % 360;
}

void angle360_iterator::operator ++()
{
    int ac = (angle + err + 900000030) / 60 % 6;
    err += angle - ac * 60;
    cur += Compass[ac];
}

spiral_iterator::spiral_iterator(coord start, int radius, bool inc_center)
    : cur(start), r(0), a1(5), a2(0), max_radius(radius)
{
    if (!inc_center)
        ++(*this);
}

void spiral_iterator::operator ++()
{
    if (++a2 >= r)
    {
        a2 = 0;
        if (++a1 >= 6)
        {
            a1 = 0;
            cur += Compass[4];
            ++r;
        }
    }

    cur += Compass[a1];
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
    for (int x = -2; x <= 2; x++)
        for (int y = -2; y <= 2; y++)
        {
            coord c(x,y);
            for (int i = 0; i <= 6; i++)
                assert(c.rotate(i).len() == c.len());
        }

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

    assert(coord(0,0).angle360() == 0);
    assert(coord(1,-1).angle360() == 30);
    assert(coord(3,-1).angle360() == 15);
    for (int i = 0; i < 6; i++)
    {
        assert(Compass[i].angle360() == i * 60);
        assert((Compass[i] + Compass[i]).angle360() == i * 60);
        assert((Compass[i] + Compass[(i+1)%6]).angle360() == i * 60 + 30);
    }

    for (spiral_iterator si(coord(0,0), 30, true); si; ++si)
    {
        int len = si->len();
        angle360_iterator a(si->angle360());
        while (len-- > 0)
            ++a;
        assert(*a == *si);
    }
}
