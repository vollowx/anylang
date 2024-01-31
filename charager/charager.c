#include "simplexnoise1234.h"
#include "tui.h"
#include <curses.h>

#define PLAYER_SYMBOL '@'
#define WORLD_WIDTH 40
#define WORLD_HEIGHT 20
#define ERR_TERM_TOO_SMALL "minimum terminal size: 89x21"

struct Tile {
  short ground;
  short object;
};

char render_tile(struct Tile tile) {
  attrset(0);
  if (tile.object != 0) {
    switch (tile.object) {
    case 1: // stone
      attron(COLOR_PAIR(5));
      return 's';
    case 2: // tree
      attron(COLOR_PAIR(8));
      return 't';
    case 3: // flower
      attron(COLOR_PAIR(7));
      return '*';
    default:
      attron(COLOR_PAIR(4));
      return '!';
    }
  } else {
    switch (tile.ground) {
    case 0:
      attron(COLOR_PAIR(2));
      return '.';
    case 1:
    case 2:
    case 3:
      return '#';
    default:
      attron(COLOR_PAIR(4));
      return '?';
    }
  }
}

struct Player {
  unsigned y;
  unsigned x;
  unsigned health;
  unsigned hunger;
};

struct Preferences {
  bool debug;
};

struct World {
  unsigned width;
  unsigned height;
  struct Tile map[WORLD_HEIGHT][WORLD_WIDTH];
  struct Player player;
  struct Preferences config;
};

void init_world(struct World *world) {
  world->width = WORLD_WIDTH;
  world->height = WORLD_HEIGHT;
  for (int y = 0; y < world->height; y++)
    for (int x = 0; x < world->width; x++)
      world->map[y][x].ground = world->map[y][x].object = 0;
  for (int y = 0; y < world->height; y++)
    for (int x = 0; x < world->width; x++)
      world->map[y][x].ground = (int)(snoise2(x / 10.0, y / 10.0) * 2.5) + 1;
  world->player.x = world->width / 2;
  world->player.y = world->height / 2;
  world->player.health = 8;
  world->player.hunger = 7;
  world->config.debug = false;
}

int ch = 0;

unsigned rendered;

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

  for (int y = 0; y < world.height; y++)
    for (int x = 0; x < world.width; x++)
      mvaddch(world_box.top + y + 1, world_box.left + x + 1,
              render_tile(world.map[y][x]));

  // Player in blue
  attrset(A_BOLD);
  attron(COLOR_PAIR(6));
  mvaddch(world_box.top + world.player.y + 1,
          world_box.left + world.player.x + 1, PLAYER_SYMBOL);
  attroff(COLOR_PAIR(6));
  attrset(A_NORMAL);

  struct BoxConf status_box = {
      .left = world_box.left + world_box.width,
      .top = world_box.top,
      .width = 24,
      .height = 6,
      .title = "status",
      .title_align = "left",
  };
  render_box(status_box);

  mvprintw(status_box.top + 1, status_box.left + 2, "health: ");
  attron(COLOR_PAIR(4));
  mvprintw(status_box.top + 1, status_box.left + 11, "%s",
           render_progressbar(world.player.health, 10));
  attroff(COLOR_PAIR(4));
  mvprintw(status_box.top + 2, status_box.left + 2, "hunger: ");
  attron(COLOR_PAIR(5));
  mvprintw(status_box.top + 2, status_box.left + 11, "%s",
           render_progressbar(world.player.hunger, 10));
  attroff(COLOR_PAIR(5));

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
    attron(COLOR_PAIR(7));
    mvprintw(world_box.top + world.player.y,
             world_box.left + world.player.x + 2, "[%2d, %2d]", world.player.x,
             world.player.y);
    attroff(COLOR_PAIR(7));
  }
}

void move_player(struct World *world, int y, int x) {
  // Check if the player is trying to move out of bounds
  if (y >= 0 && y < world->height && x >= 0 && x < world->width &&
      world->map[y][x].ground != 0) {
    // Update the player's position
    world->player.y = y;
    world->player.x = x;
  }
}

int main() {
  init_ncurses();

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

    // clear();
    render_world(world);
  } while ((ch = getch()) != 'q');

  clean_ncurses();

  return 0;
}
