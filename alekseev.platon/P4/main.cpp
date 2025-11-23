#include <iostream>
#include <cctype>
#include <iomanip>

namespace alekseev {
  char* getline(std::istream& in, size_t& size) {
    bool is_skipws = in.flags() & std::ios_base::skipws;
    if (is_skipws) {
      in >> std::noskipws;
    }

    size_t cap = 16;
    char* buf = nullptr;

    try {
      buf = new char[cap];
    }
    catch (const std::bad_alloc& e) {
      std::cerr << e.what() << "\n";
      if (is_skipws) {
        in >> std::skipws;
      }
      size = 0;
      return nullptr;
    }

    size = 0;
    char c;

    try {
      while (in >> c && c != '\n') {
        if (size + 1 >= cap) {
          cap *= 2;
          char* new_buf = new char[cap];

          for (size_t i = 0; i < size; ++i) {
            new_buf[i] = buf[i];
          }

          delete[] buf;
          buf = new_buf;
        }
        buf[size++] = c;
      }
    }
    catch (const std::bad_alloc& e) {
      std::cerr << e.what() << "\n";
      delete[] buf;
      if (is_skipws) {
        in >> std::skipws;
      }
      size = 0;
      return nullptr;
    }

    buf[size] = '\0';

    if (is_skipws) {
      in >> std::skipws;
    }

    return buf;
  }

  size_t remove_vow(const char* s, char* d, size_t d_size) {
    size_t i = 0;
    for (; *s != '\0' && i < d_size - 1; ++s) {
      char c = *s;
      if (!(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
          c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')) {
        d[i++] = c;
      }
    }
    d[i] = '\0';
    return i;
  }

  size_t exc_chars(const char* s, const char* exc, char* d, size_t d_size) {
    size_t i = 0;
    for (; *s != '\0' && i < d_size - 1; ++s) {
      char c = *s;
      const char* ex = exc;
      int found = 0;

      for (; *ex != '\0'; ++ex) {
        if (c == *ex) {
          found = 1;
          break;
        }
      }

      if (!found) {
        d[i++] = c;
      }
    }
    d[i] = '\0';
    return i;
  }
}

int main() {
  using namespace alekseev;

  std::cout << "enter first string fot var 15 and 18: ";
  size_t size1 = 0;
  char* input1 = getline(std::cin, size1);

  if (!input1 || size1 == 0) {
    std::cerr << "Error: empty input\n";
    if (input1) delete[] input1;
    return 1;
  }

  std::cout << "enter second string for var 15: ";
  size_t size2 = 0;
  char* input2 = getline(std::cin, size2);

  if (!input2) {
    delete[] input1;
    return 1;
  }

  char* result1 = nullptr;
  try {
    result1 = new char[size1 + 1];
  }
  catch (const std::bad_alloc& e) {
    std::cerr << e.what() << "\n";
    delete[] input1;
    delete[] input2;
    return 1;
  }

  remove_vow(input1, result1, size1 + 1);
  std::cout << "var 18: " << result1 << "\n";
  delete[] result1;

  char* result2 = nullptr;
  try {
    result2 = new char[size1 + 1];
  }
  catch (const std::bad_alloc& e) {
    std::cerr <<  e.what() << "\n";
    delete[] input1;
    delete[] input2;
    return 1;
  }

  exc_chars(input1, input2, result2, size1 + 1);
  std::cout << "var 15: " << result2 << "\n";

  delete[] result2;
  delete[] input1;
  delete[] input2;

  return 0;
}
