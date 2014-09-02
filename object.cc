#include "adalyah.h"
#include <set>
#include <unordered_map>
#include <vector>
#include "object.h"

vector<obj_t> Objs;
static unordered_map<coord, set<int> > omap;

int add_obj(obj_type type, coord pos)
{
    int oid = Objs.size();
    Objs.emplace_back();

    Objs[oid].pos = pos;
    Objs[oid].type = type;
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

    // Shorten the vector.
    if (oid < (int)Objs.size() - 1)
    {
        int last_oid = Objs.size() - 1;
        unmap_obj(last_oid);
        Objs[oid] = Objs[last_oid];
        omap[Objs[oid].pos].insert(oid);
    }
    Objs.pop_back();
}

void move_obj(int oid, coord newpos)
{
    assert(oid >= 0);
    assert(oid < (int)Objs.size());

    unmap_obj(oid);
    Objs[oid].pos = newpos;
    omap[Objs[oid].pos].insert(oid);
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
    }
    return true;
}
