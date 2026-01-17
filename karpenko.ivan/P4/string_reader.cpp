#include "string_reader.hpp"
#include <iostream>
#include <cstring>
#include <cstddef>

char* karpenko::readStringWithAmortization(std::istream& in, size_t& size,
  bool (*checkChar)(char))
{
  const size_t INITIAL_CAPACITY = 16;
  const double GROW_FACTOR = 1.5;

  size_t capacity = INITIAL_CAPACITY;
  char* buffer = new char[capacity];

  size = 0;
  char c;

  while (in.get(c) && checkChar(c))
  {
    if (size >= capacity - 1)
    {
      size_t newCapacity = static_cast< size_t >(capacity * GROW_FACTOR);
      if (newCapacity <= capacity)
      {
        newCapacity = capacity + 1;
      }

      char* newBuffer = nullptr;
      try
      {
        newBuffer = new char[newCapacity];
      }
      catch (const std::bad_alloc&)
      {
        delete[] buffer;
        throw;
      }

      std::memcpy(newBuffer, buffer, size);
      delete[] buffer;
      buffer = newBuffer;
      capacity = newCapacity;
    }

    buffer[size++] = c;
  }

  buffer[size] = '\0';
  return buffer;
}
