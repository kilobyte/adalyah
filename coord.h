#ifndef COORD_H
#define COORD_H

struct coord
{
    int x; int y;
    coord(int cx, int cy) : x(cx), y(cy) {};

    bool operator == (const coord &c) const
    {
        return x == c.x && y == c.y;
    }

    bool operator != (const coord &c) const
    {
        return x != c.x || y != c.y;
    }

    coord &operator += (const coord &c)
    {
        x += c.x;
        y += c.y;
        return *this;
    }

    coord operator + (const coord &c) const
    {
        return coord(x + c.x, y + c.y);
    }

    coord &operator -= (const coord &c)
    {
        x -= c.x;
        y -= c.y;
        return *this;
    }

    coord operator - (const coord &c) const
    {
        return coord(x - c.x, y - c.y);
    }

    int len() const
    {
        if ((x ^ y) < 0) // different signs
            return abs(x) + abs(y);
        else
            return max(abs(x), abs(y));
    }
};

extern coord Compass[6];

void test_coord();
#endif
