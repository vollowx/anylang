#include "board.h"
#include "terminal_manager.h"

int main() {
  TerminalManager terminalManager;
  terminalManager.clearTerminal();

  const char *customMaze[10] = {
      "#####################", "#.............. ....#", "#.########.########.#",
      "#.#......#.#......#.#", "#.#.####.#.#.####.#.#", "#.#.#..#.#.#.#..#.#.#",
      "#...#..#.....#..#...#", "#.###..###.###..###.#", "#...................#",
      "#####################"};

  Board board{customMaze};

  while (true) {
    board.printBoard();
    char c = terminalManager.getInput();
    switch (c) {
    case 'k': // Move up
      board.playerMove(board.playerX, board.playerY - 1);
      break;
    case 'h': // Move left
      board.playerMove(board.playerX - 1, board.playerY);
      break;
    case 'j': // Move down
      board.playerMove(board.playerX, board.playerY + 1);
      break;
    case 'l': // Move right
      board.playerMove(board.playerX + 1, board.playerY);
      break;
    case 'q':
      terminalManager.clearTerminal();
      terminalManager.moveCursor(board.height, 0);
      return 0;
    }
  }

  return 0;
}
