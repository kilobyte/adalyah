#ifndef TERM_H
#define TERM_H
#include "colours.h"

typedef struct
{
    int sx, sy;
    int map_lines;
    int msg_lines;
    bool need_resize;
    int cl, ct;
} term_layout;
extern term_layout TermLayout;

typedef struct
{
    const char* symbol;
    rgb_t colour;
} glyph_t;

void term_getsize(void);
void term_init(void);
void term_restore(void);
void setup_signals(void);
void set_colour(rgb_t c);
void set_cursor(int x, int y);
void hide_cursor();
void show_cursor();
void draw_glyph(int x, int y, glyph_t& g);

#endif
