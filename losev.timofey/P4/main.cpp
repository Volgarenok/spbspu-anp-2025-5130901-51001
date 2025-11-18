#include<iostream>
#include<iomanip>

namespace losev {
  size_t getline(std::istream & in, char * data, size_t size);
}


int main() {
  namespace los = losev;
  char str[] = "!!!!!";
  size_t str_size = 5;
  size_t k = los::getline(std::cin, str, str_size);
  std::cout << str << "\n";
}

size_t losev::getline(std::istream & in, char * data, size_t size) {
  bool is_skips = in.flags() & std::ios_base::skipws;
  if (is_skips) {
    in >> std::noskipws;
  }
  size_t i = 0;
  for(; in && i < size; ++i) {
    in >> data[i];
  }
  if (is_skips) {
    in >> std::skipws;
  }
  return i;
}