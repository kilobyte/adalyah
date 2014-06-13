#ifndef TERM_H
#define TERM_H

typedef struct
{
    int sx, sy;
} term_layout;
extern term_layout TermLayout;

void term_getsize(void);
void term_init(void);
void term_restore(void);

#endif
