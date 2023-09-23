#ifndef BOARD_H
#define BOARD_H

#include "ascii.h"
#include "terminal_manager.h"
#include <cstring>
#include <iostream>

using namespace ascii;

class Board {
public:
  Board(const char **map) {
    height = sizeof(map[0]) + 2;
    width = strlen(map[0]);

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
            std::cout << color::WHITE; // White for borders
          } else if (current_char == '.') {
            std::cout << color::BLACK; // Black for dots
          } else if (current_char == '$') {
            std::cout << color::BLUE; // Blue for the player
          }
          // Move cursor to position (j, i)
          terminal_manager.move_cursor(i + 1, j * 2 + 1);
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
  TerminalManager terminal_manager; // Added TerminalManager as a member
};

#endif
