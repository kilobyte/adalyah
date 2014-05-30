#include "adalyah.h"
#include "test.h"

#include "coord.h"
#include "colours.h"
#include "map.h"
#include "unicode.h"

void run_tests(const char* arg)
{
    test_coord();
    test_colours();
    test_map();
    test_unicode();
}
