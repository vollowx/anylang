#ifndef BOARD_H
#define BOARD_H

#include "colors.h"
#include "terminal.h"

enum direction { UP, DOWN, LEFT, RIGHT };

class Board {
public:
  Board(const char *map[]);
  ~Board();
  void render();
  bool set_player_position(unsigned int x, unsigned int y);
  void set_player_direction(enum direction);

  // TODO: make player a class that inherit Entity which also give Ghost
  unsigned int width = 0;
  unsigned int height = 0;
  unsigned int player_x = 1;
  unsigned int player_y = 1;
  enum direction player_direction = RIGHT;

private:
  char **board;
  char **previous_board;
};

#endif
