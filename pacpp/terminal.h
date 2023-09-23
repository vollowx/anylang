#ifndef TERMINAL_MANAGER_H
#define TERMINAL_MANAGER_H

#include "ascii.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>

using namespace ascii;

class Terminal {
public:
  Terminal() { init(); }
  ~Terminal() { restore(); }

  void clear_screen() { std::cout << "\033[2J"; }

  char get_input() {
    char c;
    std::cin >> c;
    return c;
  }

  void init() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    original_termios = t;
    t.c_lflag &= ~ICANON;
    t.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    std::cout << cursor::HIDE;
  }

  void restore() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
    std::cout << cursor::SHOW;
  }

  void move_cursor(unsigned int row, unsigned int col) {
    std::cout << cursor::MOVE(row, col);
  }

private:
  struct termios original_termios;
};

#endif
