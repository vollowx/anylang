#ifndef TERMINAL_H
#define TERMINAL_H

#include <cassert>
#include <cstdint>
#include <deque>
#include <functional>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace terminal {

inline void clear_screen() { std::cout << "\033[2J"; }

inline std::pair<int, int> get_terminal_size() {
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  return {w.ws_row - 1, w.ws_col}; // Subtract 1 to give room for the UNIX
                                   // prompt at the bottom of the screen
}

static bool raw_mode = false;
inline void set_raw_mode(bool flag) {
  static struct termios old_tio;
  if (flag and !raw_mode) {
    tcgetattr(STDIN_FILENO, &old_tio);
    raw_mode = true;

    termios tio = old_tio;
    tio.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &tio);

  } else if (!flag and raw_mode) { // Restore original settings
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    raw_mode = false;
  }
}

inline void set_cursor_position(uint32_t row, uint32_t col) {
  std::cerr << "\033[" << row << ";" << col << "H";
}

inline void show_cursor(bool flag) {
  if (flag)
    std::cerr << "\033[?25h";
  else
    std::cerr << "\033[?25l";
}

// Mouse support - there may be a bug with the first click
inline static bool mouse_mode = false; // We default to not reading mouse events
inline void remind_about_callbacks(int row, int col);
// With mouse events on, if they don't set a handler for mousedown events we
// will remind them
inline static std::function<void(int, int)> mousedown_callback =
    remind_about_callbacks;
inline static std::function<void(int, int)> mouseup_callback = [](int, int) {
}; // Ignore mouseup events by default

// The default function reminds users how to set up a callback then disables
// itself
inline void remind_about_callbacks([[maybe_unused]] int row,
                                   [[maybe_unused]] int col) {
  std::cerr << "You enabled mouse events but you didn't call 'on_mousedown' to "
               "set up a callback, so enabling mouse events was pointless.\n";

  std::cerr << "Write code like this: 'void mousedown(int row, int col) { "
               "std::cout << row << \",\" << col << std::endl; }\nand then in "
               "main: on_mousedown(mousedown);\nDitto for mouseup. Then "
               "whenever the user clicks, it will call these two functions."
            << std::endl;

  std::cerr << "If you instead want to disable, for example, mouseup events, "
               "do this: on_mouseup([](int,int){});"
            << std::endl;

  // Disable reminders
  mousedown_callback = [](int, int) {};
}

// Register callback - every time there is a click it will call this function
// Once you've enabled mouse events with set_mouse_mode(true), you need to set
// up a callback
//  for mousedown and/or mouseup events.
// To set up a callback (a function that will get called when the user clicks),
// do this:
//   on_mousedown(your_function_name);
// Where your_function_name is something like this:
//   void your_function_name(int row, int col) { cout << row << "," << col <<
//   endl; }
inline void on_mousedown(std::function<void(int, int)> f) {
  mousedown_callback = f;
}
inline void on_mouseup(std::function<void(int, int)> f) {
  mouseup_callback = f;
}

// TODO: Figure out a better way of doing smcup & rmcup
// Switches between alternate buffers, like Vim
// This lets you switch the whole contents of the screen
inline void set_alternate_window(bool flag) {
  if (flag) {
    std::cerr << "[?1049h";
    std::cerr.flush();
    // std::cerr << "[?1049h" << std::endl;
  }

  else {
    std::cerr << "[22;0;0t";
    std::cerr.flush();
    // std::cerr << "[22;0;0t" << std::endl;
  }
}

// Many terminals support the ability to send mouse events
// Example: set_mouse_mode(true) will give us the ability to read clicks
inline void set_mouse_mode(bool flag) {
  if (flag) { // Enable mouse
    // Why == true? It makes the assertion failed message understandable
    assert(raw_mode == true); // Mouse can only be enabled in raw_mode
    std::cerr << "\033[?1000;1006;1015h";
  }

  else {
    // We can disable it outside of raw mode so no assert here
    std::cerr << "\033[?1000;1006;1015l";
  }
  mouse_mode = flag;
}

inline int kbhit() {
  assert(raw_mode == true); // This code only works in raw mode
  int count = 0;
  ioctl(STDIN_FILENO, FIONREAD, &count);
  return count;
}

const int ERR = -1;

const int ESC = 27; // ASCII code 27 is Escape
const int KEY_EOF = 4;

const int ARROW_UP = 156;
const int ARROW_DOWN = 157;
const int ARROW_RIGHT = 158;
const int ARROW_LEFT = 159;

const int HOME = 266;
const int INSERT = 267;

const int DEL = 268;
const int END = 269;
const int PAGE_UP = 270;
const int PAGE_DOWN = 271;

const int F1 = 315;
const int F2 = 316;
const int F3 = 317;
const int F4 = 318;

const int F5 = 319;
const int F6 = 321; // Hmm
const int F7 = 322;
const int F8 = 323;
const int F9 = 324;
const int F10 = 325;
const int F11 = 327; // Hmm
const int F12 = 328;
const int MOUSE_WHEEL_UP = 650;
const int MOUSE_WHEEL_DOWN = 658;

inline int quick_read() {
  assert(raw_mode == true); // This code only works in raw mode
  int bytes_available = kbhit();
  if (bytes_available) {
    char c = getchar();
    if (c != 27) // The escape key is ASCII code 27
      return c;
    // Happy fun escape code parsing time

    bytes_available--; // We've already pulled off the escape character
    if (!bytes_available)
      return c; // They just hit Escape in this case
    // Mouse click looks like \e[<0;3;21M and a release \e[<0;3;21. Where 2 is x
    // (from left) and 22 is y (from top)
    //[<0;111;2m

    // Grab remaining bytes into a deque
    std::deque<char> input;
    for (int i = 0; i < bytes_available; i++)
      input.push_back(getchar());

    // See if it contains the 4 byte escape sequence for a mouse event
    while (input.size() > 4) {
      auto [rows, cols] =
          get_terminal_size(); // Make sure we're returning an int in range
      if (input[0] == '[' and input[1] == '<' and input[2] == '0' and
          input[3] == ';') { // Mouse event
        for (int i = 0; i < 4; i++)
          input.pop_front();
        bool reading_col = true; // Col is read first, then row after a ;
        int temp_col = 0, temp_row = 0;
        while (input.size()) {
          c = input.front();
          input.pop_front();
          if (c == ';') {
            reading_col = false;
            continue;
          } else if (c == 'M') { // Mousedown event
            mousedown_callback(temp_row, temp_col);
          } else if (c == 'm') { // Mouseup event
            mouseup_callback(temp_row, temp_col);
          } else if (isdigit(c)) {
            int temp = c - '0';
            if (reading_col)
              temp_col = 10 * temp_col + temp;
            else
              temp_row = 10 * temp_row + temp;
            // Double clicks aren't handled, so just pretend nothing happened
            if (temp_col > cols or temp_row > rows)
              return ERR;
          }
        }

      } else
        break;
    }
    if (!input.size())
      return ERR;

    // The magic number we return is the sum of the escape sequence, which works
    // adequately

    int sum = 0;
    for (size_t i = 0; i < input.size(); i++) {
      c = input[i];
      // Special case F9 to F12 since they overlap with other F keys
      if (c == '2' and i == 1 and bytes_available == 4) {
        sum += 9;
      }
      sum += c;
    }
    return sum; // This should match ARROW_UP and so forth above
  }

  return ERR; // Nothing was read
}

} // namespace terminal

#endif
