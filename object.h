#ifndef OBJECT_H
#define OBJECT_H
#include "coord.h"
#include "map.h"

enum obj_type
{
    OBJ_PLAYER,
    OBJ_TURRET,
    OBJ_BULLET,
};

typedef struct
{
    obj_type type;
    coord pos;
} obj_t;

extern vector<obj_t> Objs;

int add_obj(obj_type type, coord pos);
void del_obj(int oid);
void move_obj(int oid, coord newpos);
bool view_obj_at(glyph_t& glyph, coord pos);
#endif
