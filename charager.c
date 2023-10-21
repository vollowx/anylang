#include "tui.h"

#define PLAYER_SYMBOL '@'
#define WORLD_WIDTH 40
#define WORLD_HEIGHT 20
#define ERR_TERM_TOO_SMALL "minimum terminal size: 89x21"

struct Player {
  unsigned y;
  unsigned x;
};

struct Preferences {
  bool debug;
};

struct World {
  unsigned width;
  unsigned height;
  struct Player player;
  struct Preferences config;
};

void init_world(struct World *world) {
  world->width = WORLD_WIDTH;
  world->height = WORLD_HEIGHT;
  world->player.x = world->width / 2;
  world->player.y = world->height / 2;
  world->config.debug = false;
}

int ch = 0;

unsigned rendered;

void init_curses() {
  initscr();            // Initialize the window
  noecho();             // Don't echo any keypresses
  curs_set(FALSE);      // Don't display a cursor
  keypad(stdscr, TRUE); // Enable keypad mode
}

void clean_curses() { endwin(); }

void render_world(struct World world) {
  rendered++;

  if (COLS < 89 || LINES < 21) {
    mvprintw(LINES / 2, (COLS - strlen(ERR_TERM_TOO_SMALL)) / 2,
             ERR_TERM_TOO_SMALL);
    mvprintw(LINES / 2 + 1, (COLS - strlen(ERR_TERM_TOO_SMALL)) / 2,
             "current: %dx%d", COLS, LINES);

    return;
  }

  struct BoxConf world_box = {
      .width = world.width + 2,
      .height = world.height + 2,
      .title = "charager",
      .title_align = "left",
      .footer = "<q> to quit",
      .footer_align = "right",
  };
  world_box.left = (COLS - world_box.width) / 2;
  world_box.top = (LINES - world_box.height) / 2;
  render_box(world_box);

  mvaddch(world_box.top + world.player.y + 1,
          world_box.left + world.player.x + 1, PLAYER_SYMBOL);

  struct BoxConf status_box = {
      .left = world_box.left + world_box.width,
      .top = world_box.top,
      .width = 24,
      .height = 6,
      .title = "status",
      .title_align = "left",
  };
  render_box(status_box);

  mvprintw(status_box.top + 1, status_box.left + 2, "health:   %s",
           render_progressbar(5, 10));
  mvprintw(status_box.top + 2, status_box.left + 2, "hunger:   %s",
           render_progressbar(3, 10));
  mvprintw(status_box.top + 3, status_box.left + 2, "thirst:   %s",
           render_progressbar(7, 10));

  if (world.config.debug) {
    struct BoxConf debug_box = {
        .left = world_box.left + world_box.width,
        .top = status_box.top + status_box.height,
        .width = 24,
        .height = 13,
        .title = "debug",
        .title_align = "left",
    };
    render_box(debug_box);

    mvprintw(debug_box.top + 1, debug_box.left + 2, "input:           %3d", ch);
    mvprintw(debug_box.top + 2, debug_box.left + 2, "world width:     %3d",
             world.width);
    mvprintw(debug_box.top + 3, debug_box.left + 2, "world height:    %3d",
             world.height);
    mvprintw(debug_box.top + 4, debug_box.left + 2, "rendered:        %3d",
             rendered);
    mvprintw(debug_box.top + 5, debug_box.left + 2, "terminal width:  %3d",
             COLS);
    mvprintw(debug_box.top + 6, debug_box.left + 2, "terminal height: %3d",
             LINES);

    // Beside the player
    mvprintw(world_box.top + world.player.y,
             world_box.left + world.player.x + 2, "[%2d, %2d]", world.player.x,
             world.player.y);
  }
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

  struct World world;
  init_world(&world);

  do {
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

    case 'r':
      init_world(&world);
      break;

    case '\\':
      world.config.debug = !world.config.debug;
      break;
    }

    clear();
    render_world(world);
  } while ((ch = getch()) != 'q');

  clean_curses();

  return 0;
}
