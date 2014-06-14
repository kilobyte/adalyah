#ifndef COLOURS_H
#define COLOURS_H

typedef struct { uint8_t r; uint8_t g; uint8_t b; } rgb_t;

rgb_t rgb(uint32_t col);
int rgb_to_col256(rgb_t c);
rgb_t blend(rgb_t a, rgb_t b, int x = 128);

void test_colours();
#endif
