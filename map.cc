#include "adalyah.h"
#include <map>
#include "los.h"
#include "map.h"
#include "term.h"

struct you You = { coord(0,0) };
vector<light_t> lights;
int empty_light = -1; // lights.radius points to the next free element

#ifdef __GXX_EXPERIMENTAL_CXX0X__
// Buggy early compilers.
static map<unsigned int, cell_t> FMap;

cell_t& fmap(coord c)
{
    unsigned int sn = ((unsigned int)c.x) << 16
                    | ((unsigned int)c.y) & 0xffff;
    return FMap[sn];
}
#else

#define SLAB_SHIFT 6
#define SLAB_SIZE (1<<SLAB_SHIFT)

typedef cell_t slab_t[SLAB_SIZE][SLAB_SIZE];
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

cell_t& fmap(coord c)
{
    unsigned int sn = slab_num(c);
    if (!FMap.count(sn))
        init_slab(sn);
    return FMap[sn][((unsigned int)c.x) % SLAB_SIZE]
                   [((unsigned int)c.y) % SLAB_SIZE];
}
#endif

int add_light(coord pos, rgb_t colour, uint8_t intensity, int radius)
{
    assert(radius >= 0);
    assert(radius <= 8); // arbitrary

    int lid;
    if (empty_light == -1)
        lid = lights.size(), lights.emplace_back();
    else
        lid = empty_light, empty_light = lights[lid].radius;

    lights[lid].pos = pos;
    lights[lid].colour = colour;
    lights[lid].intensity = intensity;
    lights[lid].radius = radius;
    for (spiral_iterator si(pos, radius, true); si; ++si)
    {
        cell_t& cell(fmap(*si));
        if (!cell.lights)
            cell.lights.reset(new set<int>);
        cell.lights->insert(lid);
    }

    return lid;
}

void del_light(int lid)
{
    assert(lid >= 0);
    assert(lid < (int)lights.size());
    for (spiral_iterator si(lights[lid].pos, lights[lid].radius, true); si; ++si)
    {
        cell_t& cell(fmap(*si));
        cell.lights.get()->erase(lid);
        if (cell.lights->empty())
            cell.lights.reset();
    }

    lights[lid].radius = empty_light;
    empty_light = lid;
}

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
            fmap(coord(x - smap[y] - 24, y - 9)).feat = f;
        }
    add_light(coord(-5,0), rgb(0xff0000), 128, 8);
    add_light(coord(5,2),  rgb(0x00ff00), 128, 8);
    add_light(coord(0,6),  rgb(0x0000ff), 128, 8);

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
            cell_t& cell(fmap(c));
            if (vision(c0, c))
            {
                rgb_t col = rgb(cell.feat == FEAT_WALL ? 0x55aaff : 0x55aa55);
                if (cell.lights)
                    for (auto i = cell.lights->cbegin();
                         i != cell.lights->cend(); ++i)
                    {
                        light_t& li(lights[*i]);
                        int dist = (c - li.pos).len();
                        // FIXME: order of lights should be irrelevant
                        col = blend(col, li.colour,
                                    (li.radius+1-dist) * li.intensity
                                    / (li.radius+1));
                    }
                set_colour(col);
            }
            else
                set_colour(rgb(0x555555));
            printf("%s", feat_glyphs[cell.feat]);
        }
        printf("\e[B\e[G");
    }
    printf("\e[?25h");
}

static void test_fmap_access(coord c)
{
    feat_t f = fmap(c).feat;
    fmap(c).feat = FEAT_FLOOR;
    assert(fmap(c).feat == FEAT_FLOOR);
    fmap(c).feat = f;
}

void test_map()
{
    test_fmap_access(coord(0,0));
    test_fmap_access(coord(100,97));
    test_fmap_access(coord(-78,-123));
}
