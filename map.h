#ifndef MAP_H
#define MAP_H
#include "coord.h"

enum feat_t
{
    FEAT_VOID,
    FEAT_WALL,
    FEAT_FLOOR,
};

feat_t& fmap(coord c);
void generate_map(void);
void draw_map(void);

void test_map();
#endif
