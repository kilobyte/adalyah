#include "adalyah.h"
#include <locale.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "keyboard.h"
#include "map.h"
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
            printf("\e[32m%x\e[1;33m¤\e[0m ", ch);
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

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");
    parse_options(argc, argv);

    term_init();
    term_getsize();
    generate_map();
    draw_map();
    term_restore();

    return 0;
}
