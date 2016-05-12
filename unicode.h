#ifndef UNICODE_H
#define UNICODE_H
#include <string>

int wctoutf8(char *d, ucs_t s);
int utf8towc(ucs_t *d, const char *s);

int strwidth(const char *s);
static inline int strwidth(const string &s)
{
    return strwidth(s.c_str());
}

string chop_string(const char *s, int width, bool spaces = true);
static inline string chop_string(const string &s, int width, bool spaces = true)
{
    return chop_string(s.c_str(), width, spaces);
}

void test_unicode();
#endif
