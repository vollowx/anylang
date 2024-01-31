#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void init_ncurses();

void clean_ncurses();

struct BoxConf {
  unsigned left;
  unsigned top;
  unsigned width;
  unsigned height;
  char *border;
  char *title;
  char *title_align;
  char *footer;
  char *footer_align;
};

void render_box(struct BoxConf config);

char *render_progressbar(int value, int max);
