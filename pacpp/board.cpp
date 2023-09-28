#include "board.h"
#include "terminal.h"
#include <cstring>
#include <iostream>

Board::Board(const char *map[]) {
  width = strlen(map[0]);
  while (strlen(map[height])) {
    height++;
  };

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

Board::~Board() {
  for (unsigned int i = 0; i < height; i++) {
    delete[] board[i];
    delete[] previous_board[i];
  }
  delete[] board;
  delete[] previous_board;
}

void Board::render() {
  for (unsigned int i = 0; i < height; i++) {
    for (unsigned int j = 0; j < width; j++) {
      char current_char = (i == player_y && j == player_x) ? '$' : board[i][j];
      if (current_char != previous_board[i][j]) {
        if (current_char == '#') { // borders
          std::cout << colors::BLACK;
        } else if (current_char == '$') { // player
          std::cout << colors::YELLOW;
        }
        terminal::set_cursor_position(i + 1, j * 2 + 1);
        std::wcout << current_char << " ";
        previous_board[i][j] = current_char;
        std::cout << colors::RESET;
      }
    }
  }
  std::cout << flush;
}

bool Board::set_player_position(unsigned int x, unsigned int y) {
  if (x < width && y < height && board[y][x] != '#') {
    player_x = x;
    player_y = y;
    return true;
  }
  return false;
}

void Board::set_player_direction(enum direction dir) { player_direction = dir; }
