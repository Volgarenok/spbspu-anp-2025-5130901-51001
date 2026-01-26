#ifndef ARRAY_UTILS_HPP
#define ARRAY_UTILS_HPP

#include <iostream>
#include <cstddef>

namespace karpenko
{
  bool isWordChar(char ch);
  bool isSpaceChar(char ch);
  bool skipSpaces(std::istream& in, char& c);
  char** readWords(std::istream& in, size_t& wordCount);
}

#endif
