#include "board.h"
#include "terminal.h"

int main() {
  Terminal terminal;
  terminal.clear_screen();

  const char *map[10] = {"#####################", "#                   #",
                         "# ######## ######## #", "# #      # #      # #",
                         "# # #### # # #### # #", "# # #  # # # #  # # #",
                         "#   #  #     #  #   #", "# ###  ### ###  ### #",
                         "#                   #", "#####################"};

  Board board{map, terminal};

  while (true) {
    board.render_board();
    char c = terminal.get_input();
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
      terminal.clear_screen();
      terminal.move_cursor(board.height, 0);
      return 0;
    }
  }

  return 0;
}
