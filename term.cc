#include "adalyah.h"
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "term.h"

static struct termios old_tattr;
term_layout TermLayout;

void term_init(void)
{
    struct termios tattr;

    if (!isatty(0)) // not a terminal
        return;

    tcgetattr(0, &old_tattr);
    tattr = old_tattr;
    /* cfmakeraw(&tattr); */
    tattr.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP
                       |INLCR|IGNCR|ICRNL|IXON);
    tattr.c_oflag &= ~OPOST;
    tattr.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
    tattr.c_cflag &= ~(CSIZE|PARENB);
    tattr.c_cflag |= CS8;

    // delete this later
    tattr.c_lflag |= ISIG;        /* allow C-c, C-\ and C-z */

    tattr.c_cc[VMIN]  = 1;
    tattr.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &tattr);
}

void term_restore(void)
{
    tcdrain(0);
    tcsetattr(0, TCSADRAIN, &old_tattr);
}

void term_getsize(void)
{
    struct winsize ts;

    if (ioctl(1, TIOCGWINSZ, &ts) || ts.ws_row <= 0 || ts.ws_col <= 0)
    {
        TermLayout.sy = 25;
        TermLayout.sx = 80;
        return;
    }
    TermLayout.sy = ts.ws_row;
    TermLayout.sx = ts.ws_col;
}
