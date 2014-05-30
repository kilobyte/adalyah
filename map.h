#ifndef MAP_H
#define MAP_H

enum feat_t
{
    FEAT_VOID,
    FEAT_WALL,
    FEAT_FLOOR,
};

feat_t& fmap(coord c);

void test_map();
#endif
