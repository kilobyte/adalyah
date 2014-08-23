#ifndef MAP_H
#define MAP_H
#include <memory>
#include <set>
#include "coord.h"
#include "colours.h"

enum feat_t
{
    FEAT_VOID,
    FEAT_WALL,
    FEAT_FLOOR,
};

typedef struct
{
    feat_t feat;
    unique_ptr<set<int> > lights;
} cell_t;

typedef struct
{
    coord pos;
    rgb_t colour;
    uint8_t intensity;
    int radius;
} light_t;

typedef struct
{
    const char* symbol;
    rgb_t colour;
} glyph_t;

cell_t& fmap(coord c);
int add_light(coord c, rgb_t colour, uint8_t intensity, int radius);
void del_light(int lid);
void move_light(int lid, coord pos);

void generate_map(void);
void draw_map(void);

struct you
{
    coord pos;
};
extern struct you You;

void test_map();
#endif
