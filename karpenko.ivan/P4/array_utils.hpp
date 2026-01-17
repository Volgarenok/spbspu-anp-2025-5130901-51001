#ifndef ARRAY_UTILS_HPP
#define ARRAY_UTILS_HPP

#include <iostream>
#include <cstddef>

namespace karpenko
{
  bool isWordChar(char ch);
  bool isSpaceChar(char ch);
  char** readWords(std::istream& in, size_t& wordCount);
  char* resizeCharArray(const char* array, size_t oldCapacity, size_t requiredSize, size_t& newCapacity);
}

#endif
