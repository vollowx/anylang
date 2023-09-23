#ifndef BOARD_H
#define BOARD_H

#include "ascii.h"
#include "terminal.h"
#include <cstring>
#include <iostream>

using namespace ascii;

class Board {
public:
  Board(const char **map, Terminal &terminal) {
    height = sizeof(map[0]) + 2;
    width = strlen(map[0]);
    terminal = terminal;

    board = new char *[height];
    previous_board = new char *[height];
    for (unsigned int i = 0; i < height; i++) {
      board[i] = new char[width];
      previous_board[i] = new char[width];
      for (unsigned int j = 0; j < width; j++) {
        board[i][j] = map[i][j];
        previous_board[i][j] = ' ';
      }
    }
  }

  ~Board() {
    for (unsigned int i = 0; i < height; i++) {
      delete[] board[i];
      delete[] previous_board[i];
    }
    delete[] board;
    delete[] previous_board;
  }

  unsigned int width;
  unsigned int height;
  char **board;
  char **previous_board;
  unsigned int player_x = 1;
  unsigned int player_y = 1;

  void render_board() {
    for (unsigned int i = 0; i < height; i++) {
      for (unsigned int j = 0; j < width; j++) {
        char current_char =
            (i == player_y && j == player_x) ? '$' : board[i][j];
        if (current_char != previous_board[i][j]) {
          // Set text color based on the character
          if (current_char == '#') {
            std::cout << color::BLACK; // for borders
          } else if (current_char == '$') {
            std::cout << color::YELLOW; // for the player
          }
          terminal.move_cursor(i + 1, j * 2 + 1);
          std::cout << current_char << " ";
          previous_board[i][j] = current_char;
          // Reset text color to default
          std::cout << color::RESET;
        }
      }
    }
    std::cout.flush();
  }

  bool move_player(unsigned int x, unsigned int y) {
    if (x < width && y < height && board[y][x] != '#') {
      player_x = x;
      player_y = y;
      return true;
    }
    return false;
  }

private:
  Terminal terminal;
};

#endif
