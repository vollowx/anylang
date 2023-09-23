#ifndef BOARD_H
#define BOARD_H

#include "color.h"
#include "terminal_manager.h"
#include <cstring>
#include <iostream>

class Board {
public:
  Board(const char **customMaze) {
    height = sizeof(customMaze[0]) + 2;
    width = strlen(customMaze[0]);

    board = new char *[height];
    previousBoard = new char *[height];
    for (unsigned int i = 0; i < height; i++) {
      board[i] = new char[width];
      previousBoard[i] = new char[width];
      for (unsigned int j = 0; j < width; j++) {
        board[i][j] = customMaze[i][j];
        previousBoard[i][j] = ' ';
      }
    }
  }

  ~Board() {
    for (unsigned int i = 0; i < height; i++) {
      delete[] board[i];
      delete[] previousBoard[i];
    }
    delete[] board;
    delete[] previousBoard;
  }

  unsigned int width;
  unsigned int height;
  char **board;
  char **previousBoard;
  unsigned int playerX = 1;
  unsigned int playerY = 1;

  void printBoard() {
    for (unsigned int i = 0; i < height; i++) {
      for (unsigned int j = 0; j < width; j++) {
        char currentChar = (i == playerY && j == playerX) ? '$' : board[i][j];
        if (currentChar != previousBoard[i][j]) {
          // Set text color based on the character
          if (currentChar == '#') {
            std::cout << color::WHITE; // White for borders
          } else if (currentChar == '.') {
            std::cout << color::BLACK; // Black for dots
          } else if (currentChar == '$') {
            std::cout << color::BLUE; // Blue for the player
          }
          // Move cursor to position (j, i)
          terminalManager.moveCursor(i + 1, j * 2 + 1);
          std::cout << currentChar << " ";
          previousBoard[i][j] = currentChar;
          // Reset text color to default
          std::cout << color::RESET;
        }
      }
    }
    std::cout.flush();
  }

  bool playerMove(unsigned int x, unsigned int y) {
    if (x < width && y < height && board[y][x] != '#') {
      playerX = x;
      playerY = y;
      return true;
    }
    return false;
  }

private:
  TerminalManager terminalManager; // Added TerminalManager as a member
};

#endif
