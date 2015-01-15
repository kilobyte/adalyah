#include "adalyah.h"
#include <sys/time.h>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include "los.h"
#include "object.h"
#include "random.h"

vector<obj_t> Objs;
static unordered_map<coord, set<int> > omap;
static int empty_object = -1; // obj.number points to the next element

static multimap<timee_t, int> events;

int add_obj(obj_type type, coord pos)
{
    int oid;
    if (empty_object == -1)
        oid = Objs.size(), Objs.emplace_back();
    else
        oid = empty_object, empty_object = Objs[oid].number;

    Objs[oid].pos = pos;
    Objs[oid].type = type;
    Objs[oid].light = -1;
    Objs[oid].next_act = 0;
    omap[pos].insert(oid);
    return oid;
}

static void unmap_obj(int oid)
{
    omap[Objs[oid].pos].erase(oid);
    if (omap[Objs[oid].pos].empty())
        omap.erase(Objs[oid].pos);
}

void del_obj(int oid)
{
    assert(oid >= 0);
    assert(oid < (int)Objs.size());

    unmap_obj(oid);
    if (Objs[oid].light != -1)
        del_light(Objs[oid].light);

    Objs[oid].number = empty_object;
    empty_object = oid;
}

void move_obj(int oid, coord newpos)
{
    assert(oid >= 0);
    assert(oid < (int)Objs.size());

    unmap_obj(oid);
    Objs[oid].pos = newpos;
    omap[Objs[oid].pos].insert(oid);
    if (Objs[oid].light != -1)
        move_light(Objs[oid].light, newpos);
}

bool view_obj_at(glyph_t& glyph, coord pos)
{
    auto o = omap.find(pos);
    if (o == omap.end())
        return false;

    auto top = o->second.begin();
    assert(top != o->second.end());

    switch (Objs[*top].type)
    {
    case OBJ_PLAYER:
        glyph.symbol = "＠";
        glyph.colour = rgb(0xaaaaaa);
        break;
    case OBJ_TURRET:
        glyph.symbol = "/\\";
        glyph.colour = rgb(0xdddddd);
        break;
    case OBJ_BULLET:
        glyph.symbol = "＊";
        glyph.colour = rgb(0x77aadd);
        break;
    case OBJ_WANDER:
        glyph.symbol = "＆";
        glyph.colour = rgb(0xddaa00);
        break;
    }
    return true;
}

timee_t next_event()
{
    auto ne = events.begin();
    if (ne == events.end())
        return TIMEE_NEVER;
    return ne->first;
}

timee_t now()
{
    struct timeval tn;
    gettimeofday(&tn, 0);
    return ((timee_t)tn.tv_sec) * TIMEE_SCALE
         + ((timee_t)tn.tv_usec) * TIMEE_SCALE / 1000000;
}

static void obj_act(int oid)
{
    assert(oid >= 0);
    assert(oid < (int)Objs.size());
    obj_t& o(Objs[oid]);

    switch (o.type)
    {
    case OBJ_TURRET:
        if (vision(o.pos, Objs[You.oid].pos))
        {
            int bid = add_obj(OBJ_BULLET, o.pos);
            Objs[bid].dir360 = (Objs[You.oid].pos - o.pos).angle360();
            #define BULLET_SPREAD 30
            Objs[bid].err360 = rnd(BULLET_SPREAD*2+1) - BULLET_SPREAD;
            schedule_obj(bid, now());
        }

        schedule_obj(oid, o.next_act + TIMEE_SCALE /2);
        break;

    case OBJ_BULLET:
        {
            int ac = (o.dir360 + o.err360 + 900000030) / 60 % 6;
            o.err360 += o.dir360 - ac * 60;
            coord npos = o.pos + Compass[ac];
            if (fmap(npos).feat == FEAT_WALL)
                return del_obj(oid);
            move_obj(oid, npos);
        }

        schedule_obj(oid, o.next_act + TIMEE_SCALE/6);
        break;

    case OBJ_WANDER:
        {
            vector<int> dirs;
            for (int i = 0; i < 6; ++i)
                if (is_passable(o.pos + Compass[i]))
                    dirs.push_back(i);
            if (dirs.size())
            {
                int d = dirs[rnd(dirs.size())];
                move_obj(oid, o.pos + Compass[d]);
            }
        }

        schedule_obj(oid, o.next_act + TIMEE_SCALE);
        break;

    default:;
    }
}

void acts()
{
    timee_t tn = now();
    while (1)
    {
        auto ne = events.begin();
        if (ne == events.end())
            return;
        if (ne->first > tn)
            return;

        obj_act(ne->second);
        events.erase(ne);
    }
}

void schedule_obj(int oid, timee_t when)
{
    Objs[oid].next_act = when;
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    events.insert(pair<timee_t, int>(when, oid));
#else
    events.emplace(when, oid);
#endif
}

static bool is_obj_passable(obj_type t)
{
    switch (t)
    {
    case OBJ_BULLET:
        return true;
    default:
        return false;
    }
}

bool is_passable(coord c)
{
    if (fmap(c).feat == FEAT_WALL)
        return false;
    auto os = omap.find(c);
    if (os == omap.end())
        return true;
    for (auto oi = os->second.begin(); oi != os->second.end(); ++oi)
        if (!is_obj_passable(Objs[*oi].type))
            return false;
    return true;
}
