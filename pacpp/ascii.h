#ifndef COLOR_H
#define COLOR_H

#include <format>
#include <string>

using namespace std;

namespace ascii {
namespace color {
const string RESET = "\033[0m";
const string BLACK = "\033[30m";   /* Black */
const string RED = "\033[31m";     /* Red */
const string GREEN = "\033[32m";   /* Green */
const string YELLOW = "\033[33m";  /* Yellow */
const string BLUE = "\033[34m";    /* Blue */
const string MAGENTA = "\033[35m"; /* Magenta */
const string CYAN = "\033[36m";    /* Cyan */
const string WHITE = "\033[37m";   /* White */
const string BOLD = "\033[1m";     /* Bold */
} // namespace color
namespace cursor {
const string HIDE = "\033[?25l";
const string SHOW = "\033[?25h";
string MOVE(unsigned int row, unsigned int col) {
  return format("\033[{};{}H", row, col);
}
} // namespace cursor
} // namespace ascii

#endif
