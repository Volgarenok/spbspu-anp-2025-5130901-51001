#include "string_reader.hpp"
#include <iostream>
#include <cstring>
#include <cstddef>

namespace karpenko
{
  char* read_string_with_amortization(std::istream& in, size_t& size,
    bool (*check_char)(char))
  {
    const size_t INITIAL_CAPACITY = 16;
    const double GROW_FACTOR = 1.5;

    size_t capacity = INITIAL_CAPACITY;
    char* buffer = new char[capacity];

    size = 0;
    char c;

    while (in.get(c) && check_char(c))
    {
      if (size >= capacity - 1)
      {
        size_t new_capacity = static_cast< size_t >(capacity * GROW_FACTOR);
        if (new_capacity <= capacity)
        {
          new_capacity = capacity + 1;
        }

        char* new_buffer = nullptr;
        try
        {
          new_buffer = new char[new_capacity];
        }
        catch (const std::bad_alloc&)
        {
          delete[] buffer;
          throw;
        }

        std::memcpy(new_buffer, buffer, size);
        delete[] buffer;
        buffer = new_buffer;
        capacity = new_capacity;
      }

      buffer[size++] = c;
    }

    buffer[size] = '\0';
    return buffer;
  }
}
