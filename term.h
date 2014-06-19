#ifndef TERM_H
#define TERM_H
#include "colours.h"

typedef struct
{
    int sx, sy;
    int map_lines;
    int msg_lines;
    bool need_resize;
} term_layout;
extern term_layout TermLayout;

void term_getsize(void);
void term_init(void);
void term_restore(void);
void setup_signals(void);
void set_colour(rgb_t c);

#endif
