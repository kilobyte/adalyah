#ifndef COORD_H
#define COORD_H
#include <functional>

struct coord
{
    int x; int y;
    explicit coord() : x(0), y(0) {};
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

    bool operator < (const coord &c) const
    {
        return y < c.y || y == c.y && x < c.x;
    }

    bool operator > (const coord &c) const
    {
        return y > c.y || y == c.y && x > c.x;
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
    int angle360() const;
};

// This syntax for defining hashes on custom objects is beyond retarded.
namespace std
{
template<> struct hash<coord>
{
    size_t operator()(const coord &c) const
    {
        return ((size_t)(uint32_t)c.x) << (sizeof(size_t)>=8 ? 32 : 16)
              ^ (size_t)(uint32_t)c.y;
    }
};
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

class spiral_iterator : public iterator<forward_iterator_tag, coord>
{
    public:
        spiral_iterator(coord center = coord(0,0), int radius = 0x7fffffff,
                        bool inc_center = false);
        coord operator *() const { return cur; }
        const coord* operator ->() const { return &cur; }
        void operator ++();
        void operator ++(int) { ++(*this); }
        operator bool() const { return r <= max_radius; }
    private:
        coord cur;
        int r, a1, a2;
        int max_radius;
};

void test_coord();
#endif
