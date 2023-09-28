#include "board.h"
#include "terminal.h"
#include <csignal>

void bailout() {
  terminal::set_raw_mode(false);
  terminal::show_cursor(true);
  terminal::clear_screen();
}
void interrupt_handler(int x) { std::exit(0); }

int main() {
  using namespace colors;
  using namespace std;
  using namespace terminal;

  atexit(bailout);
  signal(SIGINT, interrupt_handler);

  const auto [ROWS, COLS] = get_terminal_size();

  set_raw_mode(true);
  show_cursor(false);

  cout << "Your terminal has: " << YELLOW << ROWS << RESET << " rows and "
       << YELLOW << COLS << RESET << " cols.\n";
  cout << "Two player Tron-like game. One person clicks to move, the "
          "other uses the arrow keys.\n";
  cout << "Hit space to continue\n";

  while (quick_read() != ' ')
    usleep(10'000);

  clear_screen();

  const char *map[10] = {"#####################", "#                   #",
                         "# ######## ######## #", "# #      # #      # #",
                         "# # #### # # #### # #", "# # #  # # # #  # # #",
                         "#   #  #     #  #   #", "# ###  ### ###  ### #",
                         "#                   #", "#####################"};

  Board board{map};

  board.render();

  while (true) {
    int c = quick_read();
    switch (c) {
    case ARROW_UP:
    case 'k':
    case 'w': // Move up
      board.move_player(board.player_x, board.player_y - 1);
      board.render();
      break;
    case ARROW_LEFT:
    case 'h':
    case 'a': // Move left
      board.move_player(board.player_x - 1, board.player_y);
      board.render();
      break;
    case ARROW_DOWN:
    case 'j':
    case 's': // Move down
      board.move_player(board.player_x, board.player_y + 1);
      board.render();
      break;
    case ARROW_RIGHT:
    case 'l':
    case 'd': // Move right
      board.move_player(board.player_x + 1, board.player_y);
      board.render();
      break;
    case 'q':
      return 0;
    case ERR:
      usleep(10'000);
    }
  }

  return 0;
}
