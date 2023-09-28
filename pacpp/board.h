#ifndef BOARD_H
#define BOARD_H

#include "colors.h"
#include "terminal.h"

class Board {
public:
  Board(const char **map);
  ~Board();
  void render();
  bool move_player(unsigned int x, unsigned int y);

  unsigned int width;
  unsigned int height;
  unsigned int player_x = 1;
  unsigned int player_y = 1;

private:
  char **board;
  char **previous_board;
};

#endif
