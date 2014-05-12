#include "adalyah.h"
#include <wchar.h>
#include "unicode.h"

// there must be at least 4 bytes free, NOT CHECKED!
int wctoutf8(char *d, ucs_t s)
{
    if (s < 0x80)
    {
        d[0] = s;
        return 1;
    }
    if (s < 0x800)
    {
        d[0] = ( s >>  6)         | 0xc0;
        d[1] = ( s        & 0x3f) | 0x80;
        return 2;
    }
    if (s < 0x10000)
    {
        d[0] = ( s >> 12)         | 0xe0;
        d[1] = ((s >>  6) & 0x3f) | 0x80;
        d[2] = ( s        & 0x3f) | 0x80;
        return 3;
    }
    if (s < 0x110000)
    {
        d[0] = ( s >> 18)         | 0xf0;
        d[1] = ((s >> 12) & 0x3f) | 0x80;
        d[2] = ((s >>  6) & 0x3f) | 0x80;
        d[3] = ( s        & 0x3f) | 0x80;
        return 4;
    }
    // Invalid char marker (U+FFFD).
    d[0] = 0xef;
    d[1] = 0xbf;
    d[2] = 0xbd;
    return 3;
}

int utf8towc(ucs_t *d, const char *s)
{
    if (*s == 0)
    {
        *d = 0;
        return 0;
    }
    if (!(*s & 0x80))
    {
        *d = *s;
        return 1;
    }
    if ((*s & 0xc0) == 0x80)
    {   // bare tail, invalid
        *d = 0xFFFD;
        int bad = 0;
        do bad++; while ((s[bad] & 0xc0) == 0x80);
        return bad;
    }

    int cnt;
    ucs_t c;
    if ((*s & 0xe0) == 0xc0)
        cnt=2, c = *s & 0x1f;
    else if ((*s & 0xf0) == 0xe0)
        cnt=3, c = *s & 0x0f;
    else if ((*s & 0xf8) == 0xf0)
        cnt=4, c =*s & 0x07;
    /* valid UTF-8, invalid Unicode
    else if ((*s & 0xfc) == 0xf8)
        cnt=5, c = *s & 0x03;
    else if ((*s & 0xfe) == 0xfc)
        cnt=6, c = *s & 0x01;
    */
    else
    {   // 0xfe or 0xff, invalid
        *d = 0xFFFD;
        return 1;
    }

    for (int i = 1;  i < cnt; i++)
    {
        if ((s[i] & 0xc0) != 0x80)
        {   // only tail characters are allowed here, invalid
            *d = 0xFFFD;
            return i;
        }
        c = (c << 6) | (s[i] & 0x3f);
    }

    if (c < 0xA0                        // illegal characters
        || (c >= 0xD800 && c <= 0xDFFF) // UTF-16 surrogates
        || (cnt == 3 && c < 0x800)      // overlong characters
        || (cnt == 4 && c < 0x10000)    // overlong characters
        || c > 0x10FFFF)                // outside Unicode
    {
        c = 0xFFFD;
    }
    *d = c;
    return cnt;
}

int strwidth(const char *s)
{
    ucs_t c;
    int w = 0;

    while (int l = utf8towc(&c, s))
    {
        s += l;
        int cw = wcwidth(c);
        if (cw != -1) // shouldn't ever happen
            w += cw;
    }

    return w;
}

string chop_string(const char *s, int width, bool spaces)
{
    const char *s0 = s;
    ucs_t c;

    while (int clen = utf8towc(&c, s))
    {
        int cw = wcwidth(c);
        // Due to combining chars, we can't stop at merely reaching the
        // target width, the next character needs to exceed it.
        if (cw > width) // note: a CJK character might leave one space left
            break;
        if (cw >= 0) // should we assert on control chars instead?
            width -= cw;
        s += clen;
    }

    if (spaces && width)
        return string(s0, s - s0) + string(width, ' ');
    return string(s0, s - s0);
}

void test_unicode()
{
    assert(strwidth("abc") == 3);
    assert(strwidth("a⃣") == 1);
    assert(strwidth("⿄") == 2);

    assert(chop_string("abc", 2) == "ab");
    assert(chop_string("⼮⼮", 3) == "⼮ ");
    assert(chop_string("⺙⃔⺙⃕", 3) == "⺙⃔ ");
}
