#ifndef OBJECT_H
#define OBJECT_H
#include "coord.h"
#include "map.h"
#include "term.h"

enum obj_type
{
    OBJ_PLAYER,
    OBJ_TURRET,
    OBJ_BULLET,
    OBJ_WANDER,
};

typedef int64_t timee_t;
#define TIMEE_NEVER 0x7000000000000000
#define TIMEE_SCALE 1000000

typedef struct
{
    obj_type type;
    coord pos;
    int number;
    int light;
    timee_t next_act;
} obj_t;

extern vector<obj_t> Objs;

int add_obj(obj_type type, coord pos);
void del_obj(int oid);
void move_obj(int oid, coord newpos);
bool view_obj_at(glyph_t& glyph, coord pos);

timee_t next_event();
timee_t now();

void schedule_obj(int oid, timee_t when);
void acts();
#endif
