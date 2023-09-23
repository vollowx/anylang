#ifndef TERMINAL_MANAGER_H
#define TERMINAL_MANAGER_H

#include "color.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>

class TerminalManager {
public:
  TerminalManager() { initTerminal(); }
  ~TerminalManager() { closeTerminal(); }

  void clearTerminal() {
    std::cout << "\033[2J"; // Clear the entire screen
  }

  char getInput() {
    char c;
    std::cin >> c;
    return c;
  }

  void initTerminal() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    originalTermios = t;
    t.c_lflag &= ~ICANON;
    t.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    std::cout << "\033[?25l"; // Hide the cursor
  }

  void closeTerminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
    std::cout << "\033[?25h"; // Show the cursor
  }

  void moveCursor(unsigned int row, unsigned int col) {
    std::cout << "\033[" << row << ";" << col << "H";
  }

private:
  struct termios originalTermios;
};

#endif
