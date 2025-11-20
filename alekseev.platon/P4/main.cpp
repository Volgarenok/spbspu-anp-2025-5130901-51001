#include <iostream>
#include <cctype>

namespace alekseev {
  const size_t INIT_CAP = 16;

  char* getLine(std::istream& in, size_t& size)
  {
    bool isSkipws = in.flags() & std::ios_base::skipws;
    if (isSkipws) {
      in >> std::noskipws;
    }

    size_t capacity = INIT_CAP;
    char* buffer = nullptr;

    try {
      buffer = new char[capacity];
    } catch (const std::bad_alloc& e) {
      std::cerr << "memory: " << e.what() << "\n";
      if (isSkipws) {
        in >> std::skipws;
      }
      size = 0;
      return nullptr;
    }

    size = 0;
    char currentChar;

    try {
      while (in >> currentChar && currentChar != '\n') {
        if (size + 1 >= capacity) {
          capacity *= 2;
          char* newBuffer = new char[capacity];

          for (size_t i = 0; i < size; ++i) {
            newBuffer[i] = buffer[i];
          }

          delete[] buffer;
          buffer = newBuffer;
        }
        buffer[size++] = currentChar;
      }
    } catch (const std::bad_alloc& e) {
      std::cerr << "memory: " << e.what() << "\n";
      delete[] buffer;
      if (isSkipws) {
        in >> std::skipws;
      }
      size = 0;
      return nullptr;
    }

    buffer[size] = '\0';

    if (isSkipws) {
      in >> std::skipws;
    }

    return buffer;
  }
}

int main()
{
  using namespace alekseev;

  std::cout << "enter first string fot var 15 and 18: ";
  size_t size1 = 0;
  char* input1 = getLine(std::cin, size1);

  if (!input1) {
    return 1;
  }

  std::cout << "enter second string for var 15: ";
  size_t size2 = 0;
  char* input2 = getLine(std::cin, size2);

  if (!input2) {
    delete[] input1;
    return 1;
  }


  delete[] input1;
  delete[] input2;

  return 0;
}
