#ifndef COLOR_H
#define COLOR_H

#include <string>

namespace color {
const std::string RESET = "\033[0m";
const std::string BLACK = "\033[30m";    /* Black */
const std::string RED = "\033[31m";      /* Red */
const std::string GREEN = "\033[32m";    /* Green */
const std::string YELLOW = "\033[33m";   /* Yellow */
const std::string BLUE = "\033[34m";     /* Blue */
const std::string MAGENTA = "\033[35m";  /* Magenta */
const std::string CYAN = "\033[36m";     /* Cyan */
const std::string WHITE = "\033[37m";    /* White */
const std::string BOLDBLACK = "\033[1m"; /* Bold */
} // namespace color

#endif
