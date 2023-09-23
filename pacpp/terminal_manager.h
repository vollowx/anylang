#ifndef TERMINAL_MANAGER_H
#define TERMINAL_MANAGER_H

#include "color.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>

class TerminalManager {
public:
  TerminalManager() { init(); }
  ~TerminalManager() { restore(); }

  void clear_screen() { std::cout << "\033[2J"; }

  char getInput() {
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
    std::cout << "\033[?25l"; // Hide the cursor
  }

  void restore() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
    std::cout << "\033[?25h"; // Show the cursor
  }

  void move_cursor(unsigned int row, unsigned int col) {
    std::cout << "\033[" << row << ";" << col << "H";
  }

private:
  struct termios original_termios;
};

#endif
