#include "adalyah.h"
#include <set>
#include <unordered_map>
#include <vector>
#include "object.h"

static vector<obj_t> objs;
static unordered_map<coord, set<int> > omap;

int add_obj(coord pos)
{
    int oid = objs.size();
    objs.emplace_back();

    objs[oid].pos = pos;
    omap[pos].insert(oid);
    return oid;
}

static void unmap_obj(int oid)
{
    omap[objs[oid].pos].erase(oid);
    if (omap[objs[oid].pos].empty())
        omap.erase(objs[oid].pos);
}

void del_obj(int oid)
{
    assert(oid >= 0);
    assert(oid < (int)objs.size());

    unmap_obj(oid);

    // Shorten the vector.
    if (oid < (int)objs.size() - 1)
    {
        int last_oid = objs.size() - 1;
        unmap_obj(last_oid);
        objs[oid] = objs[last_oid];
        omap[objs[oid].pos].insert(oid);
    }
    objs.pop_back();
}

bool view_obj_at(glyph_t& glyph, coord pos)
{
    auto o = omap.find(pos);
    if (o == omap.end())
        return false;

    glyph.symbol = "＊";
    glyph.colour = rgb(0xdddddd);
    return true;
}
