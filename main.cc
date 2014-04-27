#include "adalyah.h"
#include <locale.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "test.h"

static struct option options[] =
{
    { "test",            2, NULL, 't', },
    {0},
};

static void parse_options(int argc, char **argv)
{
    while (1)
        switch (getopt_long(argc, argv, "t::", options, NULL))
        {
        case -1:
            return;
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

    return 0;
}
