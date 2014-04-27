#include "adalyah.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#undef die
NORETURN die(const char *file, unsigned line, const char *format, ...)
{
    va_list args;
    char *msg;

    va_start(args, format);
    vasprintf(&msg, format, args);
    va_end(args);

    fprintf(stderr, "ERROR in '%s' at line %u: %s", file, line, msg);
    exit(1);
}

NORETURN assert_failed(const char *file, unsigned line, const char *err)
{
    fprintf(stderr, "ASSERT FAILED at %s line %u: %s\n", file, line, err);
    exit(1);
}
