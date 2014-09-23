#include "adalyah.h"
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include "term.h"

static struct termios old_tattr;
term_layout TermLayout;
static int prev_col256;
static struct { int x; int y; } prev_cursor;
static bool term_inited = false;
static vector<vector<glyph_t> > screen;

void term_init(void)
{
    struct termios tattr;

    term_inited = true;
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

    printf("\e[0m");
    prev_col256 = -1;
}

void term_restore(void)
{
    fflush(stdout);
    if (!term_inited)
        return;

    tcdrain(0);
    tcsetattr(0, TCSADRAIN, &old_tattr);
    printf("\e[?25h\e[0m");
    fflush(stdout);
}

void term_getsize(void)
{
    struct winsize ts;

    TermLayout.need_resize = false;
    if (ioctl(1, TIOCGWINSZ, &ts) || ts.ws_row <= 0 || ts.ws_col <= 0)
    {
        TermLayout.sy = 25;
        TermLayout.sx = 80;
    }
    else
    {
        TermLayout.sy = ts.ws_row;
        TermLayout.sx = ts.ws_col;
    }

    TermLayout.map_lines = TermLayout.sy*2/3 + 3;
    TermLayout.msg_lines = max(TermLayout.sy - TermLayout.map_lines, 1);

    TermLayout.cl = -((TermLayout.sx - 2) / 4);
    TermLayout.ct = -((TermLayout.map_lines - 1) / 2);
    screen.resize(TermLayout.map_lines);
    for (int i = 0; i < TermLayout.map_lines; ++i)
    {
        screen[i].resize(TermLayout.sx >> 1);
        memset(screen[i].data(), 0, TermLayout.sx >> 1);
    }
}

static void sigwinch(int dummy)
{
    TermLayout.need_resize = true;
}

void setup_signals(void)
{
    struct sigaction sa;
    sa.sa_handler = sigwinch;
    sa.sa_flags   = 0;
    sigaction(SIGWINCH, &sa, nullptr);
}

void set_colour(rgb_t c)
{
    int nc = rgb_to_col256(c);
    if (prev_col256 == nc)
        return;

    prev_col256 = nc;
    printf("\e[38;5;%dm", nc);
}

void set_cursor(int x, int y)
{
    if (prev_cursor.x == x && prev_cursor.y == y)
        return;
    prev_cursor.x = x;
    prev_cursor.y = y;
    printf("\e[%d;%dH", y+1, x+1);
}

void draw_glyph(int x, int y, glyph_t& g)
{
    int odd = y&1;
    x -= TermLayout.cl + ((y-1)>>1);
    y -= TermLayout.ct;
    assert(x >= 0);
    assert(y >= 0);
    assert(x < TermLayout.sx >> 1);
    assert(y < TermLayout.map_lines);

    glyph_t& og(screen[y][x]);
    if (og.colour.r == g.colour.r && og.colour.g == g.colour.g
        && og.colour.b == g.colour.b && og.symbol == g.symbol)
    {
        return;
    }

    set_cursor(x * 2 + odd, y);
    set_colour(g.colour);
    printf("%s", g.symbol);
    og.colour = g.colour;
    og.symbol = g.symbol;
    prev_cursor.x += 2;
}

void hide_cursor()
{
    printf("\e[?25l");
    prev_cursor.x = -1;
    prev_cursor.y = -1;
}

void show_cursor()
{
    printf("\e[?25h");
}
