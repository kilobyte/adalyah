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
}
