#include "adalyah.h"
#include "test.h"

#include "coord.h"
#include "unicode.h"

void run_tests(const char* arg)
{
    test_coord();
    test_unicode();
}
