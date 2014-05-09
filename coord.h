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

    coord rotate(int dir) const;
    int dir() const;
};

extern coord Compass[6];

class angle360_iterator : public iterator<forward_iterator_tag, coord>
{
    public:
        angle360_iterator(int angle, coord start = coord(0,0));
        coord operator *() const { return cur; }
        const coord* operator ->() const { return &cur; }
        void operator ++();
        void operator ++(int) { ++(*this); }
    private:
        coord cur;
        int angle, err;
};

void test_coord();
#endif
