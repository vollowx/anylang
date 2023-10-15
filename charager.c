#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define PLAYER_SYMBOL '@'
#define WORLD_WIDTH 40
#define WORLD_HEIGHT 20

struct Player {
  int y;
  int x;
};

struct World {
  int width;
  int height;
  struct Player player;
};

int ch = 0;

void init_curses() {
  initscr();            // Initialize the window
  noecho();             // Don't echo any keypresses
  curs_set(FALSE);      // Don't display a cursor
  keypad(stdscr, TRUE); // Enable keypad mode
}

void clean_curses() { endwin(); }

struct BoxConfiguration {
  int x;
  int y;
  int width;
  int height;
  char *border;
  char *title;
  char *title_align;
  char *footer;
  char *footer_align;
};

void render_box(struct BoxConfiguration config) {
  int width = config.width + 2;
  int height = config.height + 2;
  // Draw the border
  for (int i = 1; i < height - 1; i++) {
    mvaddch(config.y + i, config.x, ACS_VLINE);
    mvaddch(config.y + i, config.x + width - 1, ACS_VLINE);
  }
  for (int i = 1; i < width - 1; i++) {
    mvaddch(config.y, config.x + i, ACS_HLINE);
    mvaddch(config.y + height - 1, config.x + i, ACS_HLINE);
  }
  mvaddch(config.y, config.x, ACS_ULCORNER);
  mvaddch(config.y, config.x + width - 1, ACS_URCORNER);
  mvaddch(config.y + height - 1, config.x, ACS_LLCORNER);
  mvaddch(config.y + height - 1, config.x + width - 1, ACS_LRCORNER);

  // Draw the title
  if (config.title != NULL) {
    if (strcmp(config.title_align, "left") == 0) {
      mvprintw(config.y, config.x + 2, " %s ", config.title);
    } else if (strcmp(config.title_align, "center") == 0) {
      mvprintw(config.y, config.x + (width - strlen(config.title)) / 2, " %s ",
               config.title);
    } else if (strcmp(config.title_align, "right") == 0) {
      mvprintw(config.y, config.x + width - 3 - strlen(config.title), " %s ",
               config.title);
    }
  }

  // Draw the footer
  if (config.footer != NULL) {
    if (strcmp(config.footer_align, "left") == 0) {
      mvprintw(config.y + height - 1, config.x + 2, " %s ", config.footer);
    } else if (strcmp(config.footer_align, "center") == 0) {
      mvprintw(config.y + height - 1,
               config.x + (width - strlen(config.footer)) / 2, " %s ",
               config.footer);
    } else if (strcmp(config.footer_align, "right") == 0) {
      mvprintw(config.y + height - 1,
               config.x + width - 3 - strlen(config.footer), " %s ",
               config.footer);
    }
  }
}

struct BoxRect {
  int start_x;
  int start_y;
  int end_x;
  int end_y;
};

struct BoxRect calc_box_rect(struct BoxConfiguration config) {
  struct BoxRect rect = {
      .start_x = config.x,
      .start_y = config.y,
      .end_x = config.x + config.width + 2,
      .end_y = config.y + config.height + 2,
  };
  return rect;
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

void render_world(struct World world) {
  struct BoxConfiguration world_box = {
      .x = 0,
      .y = 0,
      .width = world.width,
      .height = world.height,
      .title = "Charager",
      .title_align = "left",
      .footer = "Press 'q' to quit",
      .footer_align = "right",
  };
  struct BoxRect world_rect = calc_box_rect(world_box);
  world_box.x = (COLS - world_rect.end_x) / 2;
  world_box.y = (LINES - world_rect.end_y) / 2;
  struct BoxConfiguration status_box = {
      .x = world_rect.end_x,
      .y = world_rect.start_y,
      .width = 24,
      .height = 6,
      .title = "Status",
      .title_align = "left",
  };
  struct BoxRect status_rect = calc_box_rect(status_box);
  struct BoxConfiguration debug_box = {
      .x = world_rect.end_x,
      .y = status_rect.end_y,
      .width = 24,
      .height = 13,
      .title = "Debug",
      .title_align = "left",
  };

  render_box(world_box);
  render_box(status_box);
  render_box(debug_box);

  // Render the player
  mvaddch(world_rect.start_y + world.player.y + 1,
          world_rect.start_x + world.player.x + 1, PLAYER_SYMBOL);

  // Render the status
  mvprintw(status_box.y + 1, status_box.x + 2, "Health:   %s",
           render_progressbar(5, 10));
  mvprintw(status_box.y + 2, status_box.x + 2, "Hunger:   %s",
           render_progressbar(3, 10));
  mvprintw(status_box.y + 3, status_box.x + 2, "Thirst:   %s",
           render_progressbar(7, 10));

  // Render the debug
  mvprintw(debug_box.y + 1, debug_box.x + 2, "Input:    %3d", ch);
  mvprintw(debug_box.y + 2, debug_box.x + 2, "World:    %3d, %3d", world.width,
           world.height);
  mvprintw(debug_box.y + 3, debug_box.x + 2, "Player:   %3d, %3d",
           world.player.y, world.player.x);
}

void move_player(struct World *world, int y, int x) {
  // Check if the player is trying to move out of bounds
  if (y >= 0 && y < world->height && x >= 0 && x < world->width) {
    // Update the player's position
    world->player.y = y;
    world->player.x = x;
  }
}

int main() {
  init_curses();

  struct World world = {
      .width = WORLD_WIDTH,
      .height = WORLD_HEIGHT,
      .player = {.y = 0, .x = 0},
  };

  while ((ch = getch()) != 'q') {
    // Clear the screen before rendering
    clear();

    // Move the player
    switch (ch) {
    case KEY_UP:
    case 'w':
    case 'k':
      move_player(&world, world.player.y - 1, world.player.x);
      break;
    case KEY_DOWN:
    case 's':
    case 'j':
      move_player(&world, world.player.y + 1, world.player.x);
      break;
    case KEY_LEFT:
    case 'a':
    case 'h':
      move_player(&world, world.player.y, world.player.x - 1);
      break;
    case KEY_RIGHT:
    case 'd':
    case 'l':
      move_player(&world, world.player.y, world.player.x + 1);
      break;
    }

    // Render the world
    render_world(world);

    // Refresh the screen
    refresh();
  }

  clean_curses();

  return 0;
}
