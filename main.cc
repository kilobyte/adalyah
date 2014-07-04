#include "adalyah.h"
#include <locale.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "keyboard.h"
#include "map.h"
#include "random.h"
#include "term.h"
#include "test.h"

static struct option options[] =
{
    { "test",            2, NULL, 't', },
    {0},
};

static NORETURN keyboard_test(void)
{
    term_init();
    term_getsize();

    while (1)
    {
        int ch = getch();
        if (ch == 27)
            printf("\e[32m%x\e[1;33mESC\e[0m ", ch);
        else if (ch < 32)
            printf("\e[32m%x-%x\e[1;33m¤\e[0m ", ch&0xff, -(ch&~0xff));
        else
            printf("\e[32m%x\e[1;31m%C\e[0m ", ch, ch);
        fflush(stdout);
        if (ch == 'q')
        {
            printf("\n\r");
            break;
        }
    }

    term_restore();
    exit(0);
}

static void parse_options(int argc, char **argv)
{
    while (1)
        switch (getopt_long(argc, argv, "t::k", options, NULL))
        {
        case -1:
            return;
        case 'k':
            keyboard_test();
        case 't':
            run_tests(optarg);
            exit(0);
        default:
            // getopt gave the error message already
            exit(1);
        }
}

static void draw_screen(void)
{
    printf("\e[2J");
    draw_map();
    fflush(stdout);
}

static void move_player(coord d)
{
    if (fmap(You.pos+d).feat == FEAT_WALL)
        return;
    You.pos += d;
    draw_map();
    fflush(stdout);
}

static void input(void)
{
    int ch = getch();
    switch (ch)
    {
    case 'q':       // q
    case 27:        // Esc
        term_restore();
        exit(0);

    case 'D'-0x500: // ←
    case 'D'-0x300:
    case 't'-0x200:
    case 'g':
        move_player(coord(-1,0));
        break;
    case 'C'-0x500: // →
    case 'C'-0x300:
    case 'v'-0x200:
    case 'j':
        move_player(coord(1,0));
        break;
    case  11-0x600: // Home
    case 'H'-0x200:
    case 'w'-0x200:
    case 'y': // normal keyboard
    case 't': // N900
        move_player(coord(-1,-1));
        break;
    case  44-0x600: // End
    case 'F'-0x200:
    case 'q'-0x200:
    case 'b':
        move_player(coord(0,1));
        break;
    case   5-0x600: // PgUp
    case  55-0x600:
    case 'y'-0x200:
    case 'u':
        move_player(coord(0,-1));
        break;
    case   6-0x600: // PgDn
    case  66-0x600:
    case 's'-0x200:
    case 'n': // normal keyboard
    case 'm': // N900
        move_player(coord(1,1));
        break;
    }
}

static NORETURN input_loop(void)
{
    sigset_t sigs, nosigs;
    sigemptyset(&nosigs);
    sigemptyset(&sigs);
    sigaddset(&sigs, SIGWINCH);

    while (1)
    {
        struct timespec tv;
        tv.tv_sec  = 1; // TODO: time until next event
        tv.tv_nsec = 0;

        fd_set readfdmask;
        FD_ZERO(&readfdmask);
        FD_SET(0, &readfdmask);

        sigprocmask(SIG_BLOCK, &sigs, 0);
        if (TermLayout.need_resize)
            tv.tv_sec = tv.tv_nsec = 0;
        int sres = pselect(1, &readfdmask, 0, 0, &tv, &nosigs);
        sigprocmask(SIG_UNBLOCK, &sigs, 0);

        if (TermLayout.need_resize)
        {
            term_getsize();
            draw_screen();
        }
        if (sres>=0 && FD_ISSET(0, &readfdmask))
            input();
    }
}

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");
    seed_rng();
    parse_options(argc, argv);

    term_init();
    setup_signals();
    term_getsize();
    generate_map();
    draw_screen();
    input_loop();
    term_restore();

    return 0;
}
