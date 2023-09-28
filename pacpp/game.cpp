#include "game.h"

#include <chrono>
#include <thread>

using namespace terminal;

Game::Game(Board &board) : board(board) {
  board = board;
  std::thread input_thread(&Game::input_loop, this);
  std::thread tick_thread(&Game::tick_loop, this);
  input_thread.join();
  tick_thread.join();
}

Game::~Game() {}

void Game::input_loop() {
  while (!over) {
    int c = quick_read();
    switch (c) {
    case ARROW_UP:
    case 'k':
    case 'w': // Move up
      board.set_player_direction(UP);
      board.render();
      break;
    case ARROW_LEFT:
    case 'h':
    case 'a': // Move left
      board.set_player_direction(LEFT);
      board.render();
      break;
    case ARROW_DOWN:
    case 'j':
    case 's': // Move down
      board.set_player_direction(DOWN);
      board.render();
      break;
    case ARROW_RIGHT:
    case 'l':
    case 'd': // Move right
      board.set_player_direction(RIGHT);
      board.render();
      break;
    case 'q':
      set_cursor_position(board.height + 1, 0);
      over = true;
      break;
    case ERR:
      usleep(10'000);
    }
  }
}

void Game::tick_loop() {
  bool changed = false;
  while (!over) {
    switch (board.player_direction) {
    case UP:
      changed = board.set_player_position(board.player_x, board.player_y - 1);
      break;
    case DOWN:
      changed = board.set_player_position(board.player_x, board.player_y + 1);
      break;
    case LEFT:
      changed = board.set_player_position(board.player_x - 1, board.player_y);
      break;
    case RIGHT:
      changed = board.set_player_position(board.player_x + 1, board.player_y);
      break;
    }

    if (changed) {
      board.render();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
