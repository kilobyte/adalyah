#include "adalyah.h"
#include <errno.h>
#include <unistd.h>
#include "keyboard.h"

#define GETCB \
{                                                       \
    while (1)                                           \
    {                                                   \
        int err = read(0, &cb, 1);                      \
        if (err == 1)                                   \
            break;                                      \
        else if (err == -1 && errno == EINTR)           \
            continue;                                   \
        return EOF;                                     \
    }                                                   \
}

int getch(void)
{
    string buf;
    uint8_t cb;

    GETCB;
    if (cb & 0x80)
    {
        int cnt;
        ucs_t ch;
        if ((cb & 0xe0) == 0xc0)
            cnt = 1, ch = cb & 0x1f;
        else if (cb & 0xf0 == 0xe0)
            cnt = 2, ch = cb & 0x0f;
        else if (cb & 0xf8 == 0xf0)
            cnt = 3, ch = cb & 0x07;
        else
            return 0xFFFD;
        while (cnt--)
        {
            GETCB;
            if ((cb & 0xc0) != 0x80)
                return 0xFFFD;
            ch = ch << 6 | cb & 0x3f;
        }
        return ch;
    }

    return cb;
}
