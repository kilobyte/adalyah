#include "adalyah.h"
#include "colours.h"

rgb_t rgb(uint32_t col)
{
    rgb_t c;
    c.r = (col >> 16) & 0xff;
    c.g = (col >>  8) & 0xff;
    c.b =  col        & 0xff;
    return c;
}

static rgb_t col256_to_rgb(int i)
{
    rgb_t c;
    // Standard colours.
    if (i < 8)
    {
        c.r = i&1 ? 0xaa : 0x00;
        c.g = i&2 ? 0xaa : 0x00;
        c.b = i&4 ? 0xaa : 0x00;
    }
    else if (i < 16)
    {
        c.r = i&1 ? 0xff : 0x55;
        c.g = i&2 ? 0xff : 0x55;
        c.b = i&4 ? 0xff : 0x55;
    }
    // 6x6x6 colour cube.
    else if (i < 232)
    {
        c.r = (i - 16) / 36 * 85 / 2;
        c.g = (i - 16) / 6 % 6 * 85 / 2;
        c.b = (i - 16) % 6 * 85 / 2;
    }
    // Grayscale ramp.
    else // i < 256
        c.r=c.g=c.b = i * 10 - 2312;
    return c;
}

static unsigned char rgb_to_col16(rgb_t c)
{
    unsigned char  max = c.r;
    if (c.g > max) max = c.g;
    if (c.b > max) max = c.b;
    unsigned char hue = (c.r > max / 2 ? 1 : 0)
                      | (c.g > max / 2 ? 2 : 0)
                      | (c.b > max / 2 ? 4 : 0);
    if (hue == 7 && max <= 0x55)
        return 8;
    return (max > 0xaa) ? hue | 8 : hue;
}

static int rgb_diff(rgb_t a, rgb_t b)
{
    int diff;
    diff  = (a.r > b.r) ? a.r - b.r : b.r - a.r;
    diff += (a.g > b.g) ? a.g - b.g : b.g - a.g;
    diff += (a.b > b.b) ? a.b - b.b : b.b - a.b;
    return diff;
}

int rgb_to_col256(rgb_t c)
{
    int c1 = rgb_to_col16(c);
    int bd = rgb_diff(c, col256_to_rgb(c1));
    int res = c1;
    int c2 = 16
           + min((((int)c.r) * 2 + 42) / 85, 5) * 36
           + min((((int)c.g) * 2 + 42) / 85, 5) * 6
           + min((((int)c.b) * 2 + 42) / 85, 5);
    int d = rgb_diff(c, col256_to_rgb(c2));
    if (d < bd)
        bd = d, res = c2;
    int c3 = 232 + ((int)c.r + c.g + c.b/* + 29 - 8*3*/) / 30;
    d = rgb_diff(c, col256_to_rgb(c3));
    if (d < bd)
        res = c3;
    return res;
}

rgb_t blend(rgb_t a, rgb_t b, int x)
{
    assert(x >= 0);
    assert(x <= 256);
    unsigned int y = 256 - x;
    a.r = (a.r*(unsigned int)x + b.r*y + 128) >> 8;
    a.g = (a.g*(unsigned int)x + b.g*y + 128) >> 8;
    a.b = (a.b*(unsigned int)x + b.b*y + 128) >> 8;
    return a;
}

#if 0
void display_gradient()
{
    for (int y = 0; y < 256; y+=12)
    {
        for (int x = 0; x < 256; x+=4)
        {
            rgb_t c1, c2;
            c1.r=y;
            c1.g=x;
            c1.b=(x+y)/2;
            c2.r=y+6;
            c2.g=x;
            c2.b=(x+y)/2+3;
            printf("\e[38;5;%d;48;5;%dm▀\e[0m", rgb_to_col256(c1), rgb_to_col256(c2));
        }
        printf("\n");
    }
}
#endif

void test_colours()
{
    for (int i = 0; i < 256; i++)
    {
        rgb_t c = col256_to_rgb(i);
        rgb_t d = col256_to_rgb(rgb_to_col256(c));
        assert(c.r==d.r && c.g==d.g && c.b==d.b);
    }
}
