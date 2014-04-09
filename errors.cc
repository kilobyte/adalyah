#include "adalyah.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#undef die
NORETURN void die(const char *file, int line, const char *format, ...)
{
    va_list args;
    char *msg;

    va_start(args, format);
    vasprintf(&msg, format, args);
    va_end(args);

    fprintf(stderr, "ERROR in '%s' at line %d: %s", file, line, msg);
    exit(1);
}
