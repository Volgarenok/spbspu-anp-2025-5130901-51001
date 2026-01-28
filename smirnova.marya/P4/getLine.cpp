#include "getLine.hpp"
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <istream>

namespace smirnova
{
  char* getLine(std::istream& in, size_t& size)
  {
    bool is_skipws = in.flags() & std::ios_base::skipws;
    if (is_skipws) {
      in >> std::noskipws;
    }
    size_t capacity = 16;
    size_t length = 0;

    char* str = reinterpret_cast< char* >(malloc(capacity));
    if (!str) {
      return nullptr;
    }

    char ch = '\0';
    while (in.get(ch) && ch != '\n') {
      if (length + 1 >= capacity) {
        size_t newCapacity = capacity * 2;
        char* buffer = reinterpret_cast< char* >(malloc(newCapacity));
        if (!buffer) {
          free(str);
          return 0;
        }
        for (size_t i = 0; i < length; i++) {
          buffer[i] = str[i];
        }
        free(str);
        str = buffer;
        capacity = newCapacity;
      }
      str[length++] = ch;
    }

    if (is_skipws) {
      in >> std::skipws;
    }

    char* result = reinterpret_cast< char* >(malloc(length + 1));
    if (!result) {
      free(str);
      return 0;
    }

    for (size_t i = 0; i < length; i++) {
      result[i] = str[i];
    }
    result[length] = '\0';

    free(str);
    size = length;
    return result;
  }
}
