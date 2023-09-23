#include "board.h"
#include "terminal_manager.h"

int main() {
  TerminalManager terminal_manager;
  terminal_manager.clear_screen();

  const char *map[10] = {"#####################", "#.............. ....#",
                         "#.########.########.#", "#.#......#.#......#.#",
                         "#.#.####.#.#.####.#.#", "#.#.#..#.#.#.#..#.#.#",
                         "#...#..#.....#..#...#", "#.###..###.###..###.#",
                         "#...................#", "#####################"};

  Board board{map};

  while (true) {
    board.render_board();
    char c = terminal_manager.getInput();
    switch (c) {
    case 'k': // Move up
      board.move_player(board.player_x, board.player_y - 1);
      break;
    case 'h': // Move left
      board.move_player(board.player_x - 1, board.player_y);
      break;
    case 'j': // Move down
      board.move_player(board.player_x, board.player_y + 1);
      break;
    case 'l': // Move right
      board.move_player(board.player_x + 1, board.player_y);
      break;
    case 'q':
      terminal_manager.clear_screen();
      terminal_manager.move_cursor(board.height, 0);
      return 0;
    }
  }

  return 0;
}
