using namespace std;
#include <algorithm>

#if defined(__GNUC__)
# define NORETURN __attribute__ ((noreturn)) void
#elif defined(_MSC_VER)
# define NORETURN __declspec(noreturn) void
#else
# define NORETURN void
#endif

#ifdef __GNUC__
// show warnings about the format string
# define PRINTF(x, dfmt) const char *format dfmt, ...) \
                   __attribute__((format (printf, x+1, x+2))
#else
# define PRINTF(x, dfmt) const char *format dfmt, ...
#endif

NORETURN die(const char *file, unsigned line, PRINTF(2, ));
#define die(...) die(__FILE__, __LINE__, __VA_ARGS__)

NORETURN assert_failed(const char *file, unsigned line, const char *err);
#define assert(x) do if (!(x)) assert_failed(__FILE__, __LINE__, #x); while (0)
