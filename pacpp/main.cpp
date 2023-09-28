#include "board.h"
#include "colors.h"
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

  cout << BOLD << BLUE << "Pacman game emulated in terminal.\n" << RESET;

  cout << endl;

  cout << BOLD << MAGENTA << "    <Space>" << RESET << "   start\n";
  cout << BOLD << MAGENTA << "    ↑/w/k" << RESET << "     up\n";
  cout << BOLD << MAGENTA << "    ←/a/h" << RESET << "     left\n";
  cout << BOLD << MAGENTA << "    ↓/s/j" << RESET << "     down\n";
  cout << BOLD << MAGENTA << "    →/d/l" << RESET << "     right\n";
  cout << BOLD << MAGENTA << "    q" << RESET << "         quit\n";

  cout << endl;

  cout << "{ rows: " << YELLOW << ROWS << RESET << ", cols: " << YELLOW << COLS
       << RESET << " }\n";

  while (quick_read() != ' ')
    usleep(10'000);

  clear_screen();

  // clang-format off
  const char *map[] = {
    "#####################",
    "#         #         #",
    "# ### ### # ### ### #",
    "#                   #",
    "# ## #   ###   # ## #",
    "#    ###  #  ###    #",
    "#    #    #    #    #",
    "#      #     #      #",
    "#    # ####### #    #",
    "#    #         #    #",
  };
  // clang-format on

  Board board{map};

  board.render();

  while (true) {
    int c = quick_read();
    switch (c) {
    case ARROW_UP:
    case 'k':
    case 'w': // Move up
      board.set_player_direction(UP);
      board.render();
      break;
    case ARROW_LEFT:
    case 'h':
    case 'a': // Move left
      board.set_player_direction(LEFT);
      board.render();
      break;
    case ARROW_DOWN:
    case 'j':
    case 's': // Move down
      board.set_player_direction(DOWN);
      board.render();
      break;
    case ARROW_RIGHT:
    case 'l':
    case 'd': // Move right
      board.set_player_direction(RIGHT);
      board.render();
      break;
    case 'q':
      set_cursor_position(board.height + 1, 0);
      return 0;
    case ERR:
      usleep(10'000);
    }
  }

  return 0;
}
