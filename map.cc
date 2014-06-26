#include "adalyah.h"
#include <map>
#include "los.h"
#include "map.h"
#include "term.h"

struct you You = { coord(0,0) };

#ifdef __GXX_EXPERIMENTAL_CXX0X__
// Buggy early compilers.
static map<unsigned int, feat_t> FMap;

feat_t& fmap(coord c)
{
    unsigned int sn = ((unsigned int)c.x) << 16
                    | ((unsigned int)c.y) & 0xffff;
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

static const char* smap[] =
{
    "########## ## ## ## ## ## ## ## ## ## ",
    "#.........##.##.##.##.##.##.##.##.##.# ",
    " #..####...#..........................#",
    " #..#   #...#..........................#",
    "  #.# ## #..##..........................#",
    "  #.# #.# #.#.#.........................#",
    "   #.##.# #.#..#.........................#",
    "   #..#.# #.#...#........................#",
    "    #...###.#....#........................#",
    "    #.......#.....#.......................#",
    "     #......#......#.......................#",
    "     #......#.......#......................#",
    "      #.....#........#......................#",
    "      #.....#.........#.....................#",
    "       #....#..........#.....................#",
    "       #.....#.........#.....................#",
    "        #...#............#....................#",
    "        #...###.........###...................#",
    "         ####  ##########  ####################",
};

void generate_map(void)
{
    for (int y = 0; y < (int)ARRAYSZ(smap); ++y)
        for (const char *x = smap[y]; *x; ++x)
        {
            feat_t f = FEAT_VOID;
            switch (*x)
            {
            case '.': f = FEAT_FLOOR; break;
            case '#': f = FEAT_WALL; break;
            }
            fmap(coord(x - smap[y] - 24, y - 9)) = f;
        }

    You.pos = coord(0,0);
}

static const char* feat_glyphs[] =
{
    "　",
    "░░",
    "．",
};

void draw_map(void)
{
    coord c0 = You.pos;
    int cl = -((TermLayout.sx - 2) / 4);
    int ct = -((TermLayout.map_lines - 1) / 2);
    int ch = TermLayout.map_lines;

    printf("\e[?25l\e[H");
    for (int y = ct; ch; --ch, ++y)
    {
        int cw = (TermLayout.sx - (y&1)) / 2;
        if (y&1)
            printf(" ");
        for (int x = cl + ((y-1)>>1); cw; --cw, ++x)
        {
            if (!x && !y)
            {
                set_colour(rgb(0xaaaaaa));
                printf("＠");
                continue;
            }

            coord c(c0.x + x, c0.y + y);
            feat_t f = fmap(c);
            if (vision(c0, c))
                set_colour(rgb((f == FEAT_WALL) ? 0xaaaa00 : 0xaaaaaa));
            else
                set_colour(rgb(0x555555));
            printf("%s", feat_glyphs[f]);
        }
        printf("\e[B\e[G");
    }
    printf("\e[?25h");
}

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
