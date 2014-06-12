#ifndef TERM_H
#define TERM_H

typedef struct
{
    int sx, sy;
} term_layout_t;
extern term_layout_t term_layout;

void term_getsize(void);
void term_init(void);
void term_restore(void);

#endif
