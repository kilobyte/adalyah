#include "adalyah.h"
#include <map>
#include "coord.h"
#include "map.h"

#ifdef __GXX_EXPERIMENTAL_CXX0X__
// Buggy early compilers.
static map<unsigned int, feat_t> FMap;

feat_t& fmap(coord c)
{
    unsigned int sn = ((unsigned int)c.x) << 16
                    | ((unsigned int)c.y) && 0xffff;
    return FMap[sn];
}
#else

#define SLAB_SHIFT 6
#define SLAB_SIZE (1<<SLAB_SHIFT)

typedef feat_t slab_t[SLAB_SIZE][SLAB_SIZE];
static map<unsigned int, slab_t> FMap;

static unsigned int slab_num(coord c)
{
    return c.x >> SLAB_SHIFT << 16 | c.y >> SLAB_SHIFT & 0xffff;
}

static void init_slab(unsigned int sn)
{
    slab_t &slab = FMap[sn];
    for (int x = 0; x < SLAB_SIZE; x++)
        for (int y = 0; y < SLAB_SIZE; y++)
            slab[x][y] = FEAT_VOID;
}

feat_t& fmap(coord c)
{
    unsigned int sn = slab_num(c);
    if (!FMap.count(sn))
        init_slab(sn);
    return FMap[sn][((unsigned int)c.x) % SLAB_SIZE]
                   [((unsigned int)c.y) % SLAB_SIZE];
}
#endif


static void test_fmap_access(coord c)
{
    feat_t f = fmap(c);
    fmap(c) = FEAT_FLOOR;
    assert(fmap(c) == FEAT_FLOOR);
    fmap(c) = f;
}

void test_map()
{
    test_fmap_access(coord(0,0));
    test_fmap_access(coord(100,97));
    test_fmap_access(coord(-78,-123));
}
