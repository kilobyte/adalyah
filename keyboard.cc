#include "adalyah.h"
#include <errno.h>
#include <poll.h>
#include <unistd.h>
#include <string>
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
        return KEY_EOF;                                 \
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
    if (cb == 27)
    {
        struct pollfd pfd;
        pfd.fd     = 0;
        pfd.events = POLLIN;
        if (!poll(&pfd, 1, 100))
            return 27; /* lone ESC */
        /* errors on poll are ok */
        GETCB;
        switch (cb)
        {
        case 'O':
            GETCB;
            /* ESC O A */
            return -0x200+cb;
        case '[':
            GETCB;
            switch (cb)
            {
            case 'O':
                GETCB;
                /* ESC [ O A */
                return -0x300+cb;
            case '[':
                GETCB;
                /* ESC [ [ A */
                return -0x400+cb;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                int k = cb-'0';
                while(cb>='0' && cb<='9')
                {
                    k = k*10 + cb - '0';
                    GETCB;
                }
                if (cb == '~')
                    return -0x600+k;	/* ESC [ 11 ~ */
                else
                    return -0x700+k;	/* ESC [ 11 invalid */
            /* ESC [ A */
            }
            return -0x500+cb;
        }
        return -0x800+cb;	/* ESC A -- usually alt+A */
    }

    return cb;
}
