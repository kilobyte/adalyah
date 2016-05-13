#include "adalyah.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "term.h"

#undef die
NORETURN die(const char *file, unsigned line, const char *format, ...)
{
    va_list args;
    char *msg = nullptr;

    term_restore();

    va_start(args, format);
    if (vasprintf(&msg, format, args) == -1); // null on alloc failure
    va_end(args);

    fprintf(stderr, "ERROR in '%s' at line %u: %s", file, line, msg);
    exit(1);
}

NORETURN assert_failed(const char *file, unsigned line, const char *err)
{
    term_restore();
    fprintf(stderr, "ASSERT FAILED at %s line %u: %s\n", file, line, err);
    exit(1);
}
