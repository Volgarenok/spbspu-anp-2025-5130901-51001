#ifndef ARRAY_UTILS_HPP
#define ARRAY_UTILS_HPP

#include <iostream>
#include <cstddef>

namespace karpenko
{
  bool isWordChar(char ch);
  bool isSpaceChar(char ch);
  char** readWords(std::istream& in, size_t& wordCount);

  void resizeCharArray(char*& array, size_t& capacity, size_t requiredSize);
}

#endif
