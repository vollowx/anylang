#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "terminal.h"

class Game {
public:
  Game(Board &board);
  ~Game();

  void input_loop();
  void tick_loop();

private:
  Board &board;
  bool over = false;
};

#endif
