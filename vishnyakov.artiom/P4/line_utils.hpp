#ifndef LINE_UTILS_HPP
#define LINE_UTILS_HPP
#include <iostream>

namespace vishnyakov
{
  bool isIn(char *str, char letter);
  void copy(const char *a, char *b, size_t l);
  bool isSpace(char letter);
}

#endif