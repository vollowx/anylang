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

  unsigned int width = 0;
  unsigned int height = 0;
  unsigned int player_x = 1;
  unsigned int player_y = 1;
  enum direction player_direction = LEFT;

private:
  char **board;
  char **previous_board;
};

#endif
