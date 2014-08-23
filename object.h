#ifndef OBJECT_H
#define OBJECT_H
#include "coord.h"
#include "map.h"

typedef struct
{
    coord pos;
} obj_t;

int add_obj(coord pos);
void del_obj(int oid);
bool view_obj_at(glyph_t& glyph, coord pos);
#endif
