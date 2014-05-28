#ifndef COLOURS_H
#define COLOURS_H

typedef struct { unsigned r; unsigned g; unsigned b; } rgb_t;

rgb_t blend(rgb_t a, rgb_t b, int x = 128);

void test_colours();
#endif
