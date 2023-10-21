#include "tui.h"

void render_box(struct BoxConf config) {
  config.width -= 1;
  config.height -= 1;
  // Draw the border
  for (int i = 0; i < config.height; i++) {
    mvaddch(config.top + i, config.left, ACS_VLINE);
    mvaddch(config.top + i, config.left + config.width, ACS_VLINE);
  }
  for (int i = 0; i < config.width; i++) {
    mvaddch(config.top, config.left + i, ACS_HLINE);
    mvaddch(config.top + config.height, config.left + i, ACS_HLINE);
  }
  mvaddch(config.top, config.left, ACS_ULCORNER);
  mvaddch(config.top, config.left + config.width, ACS_URCORNER);
  mvaddch(config.top + config.height, config.left, ACS_LLCORNER);
  mvaddch(config.top + config.height, config.left + config.width, ACS_LRCORNER);

  // Draw the title
  if (config.title != NULL) {
    if (strcmp(config.title_align, "left") == 0) {
      mvprintw(config.top, config.left + 2, " %s ", config.title);
    } else if (strcmp(config.title_align, "center") == 0) {
      mvprintw(config.top,
               config.left + (config.width - strlen(config.title)) / 2, " %s ",
               config.title);
    } else if (strcmp(config.title_align, "right") == 0) {
      mvprintw(config.top,
               config.left + config.width - 2 - strlen(config.title), " %s ",
               config.title);
    }
  }

  // Draw the footer
  if (config.footer != NULL) {
    if (strcmp(config.footer_align, "left") == 0) {
      mvprintw(config.top + config.height, config.left + 2, " %s ",
               config.footer);
    } else if (strcmp(config.footer_align, "center") == 0) {
      mvprintw(config.top + config.height,
               config.left + (config.width - strlen(config.footer)) / 2, " %s ",
               config.footer);
    } else if (strcmp(config.footer_align, "right") == 0) {
      mvprintw(config.top + config.height,
               config.left + config.width - 3 - strlen(config.footer), " %s ",
               config.footer);
    }
  }
}

char *render_progressbar(int value, int max) {
  char *bar = malloc(max + 1);
  for (int i = max - 1; i >= 0; i--) {
    if (i >= value) {
      bar[i] = '#';
    } else {
      bar[i] = '.';
    }
  }
  bar[max] = '\0';
  return bar;
}
