#include<iostream>
#include<iomanip>
#include<stdlib.h>

namespace losev {
  size_t getline(std::istream & in, char * data, size_t size);
  char * getline(std::istream & in, size_t & size);
}


int main() {
  namespace los = losev;
  size_t str_size = 10;
  char * str =  los::getline(std::cin, str_size);
  std::cout << str << "\n";
}

char * losev::getline(std::istream & in, size_t & size) {
  char * str = reinterpret_cast<char*>(malloc(size * sizeof(char)));
  if (!str) {
    std::cerr << "Memory allocation failed\n";
    free(str);
    return nullptr;
  }
  bool is_skips = in.flags() & std::ios_base::skipws;
  if (is_skips) {
    in >> std::noskipws;
  }
  for (size_t i = 0; in && i < size; ++i) {
    in >> str[i];
    if (i == (size - 1) && str[i] != 0) {
      char * str_new = reinterpret_cast<char*>(malloc((size + 10) * sizeof(char)));
      if (!str_new) {
        std::cerr << "Memory allocation failed\n";
        free(str_new);
        free(str);
        return nullptr;
      }
      for (size_t j = 0; j < size; j++) {
        str_new[j] = str[j];
      }
      size += 10;
      str = str_new;
    }
  }
  if (is_skips) {
    in >> std::skipws;
  }
  return str;
}

size_t losev::getline(std::istream & in, char * data, size_t size) {
  bool is_skips = in.flags() & std::ios_base::skipws;
  if (is_skips) {
    in >> std::noskipws;
  }
  size_t i = 0;
  for (; in && i < size; ++i) {
    in >> data[i];
  }
  if (is_skips) {
    in >> std::skipws;
  }
  return i;
}