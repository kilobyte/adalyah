using namespace std;

#if defined(__GNUC__)
# define NORETURN __attribute__ ((noreturn))
#elif defined(_MSC_VER)
# define NORETURN __declspec(noreturn)
#else
# define NORETURN
#endif

#ifdef __GNUC__
// show warnings about the format string
# define PRINTF(x, dfmt) const char *format dfmt, ...) \
                   __attribute__((format (printf, x+1, x+2))
#else
# define PRINTF(x, dfmt) const char *format dfmt, ...
#endif

NORETURN void die(const char *file, int line, PRINTF(2, ));
#define die(...) die(__FILE__, __LINE__, __VA_ARGS__)
