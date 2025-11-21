#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<cctype>

namespace losev {
  char * getline(std::istream & in, size_t & size);
  char * lat_rmv(const char * src, char * dest, size_t size);
}


int main() {
  namespace los = losev;
  size_t str_size = 10;
  char * str =  los::getline(std::cin, str_size);
  if (str == nullptr) {
    std::cerr << "Fail memory\n";
    return 1;
  }
  char * strForFirstEx = reinterpret_cast<char*>(malloc(str_size * sizeof(char)));
  if (!strForFirstEx) {
    std::cerr << "Memory allocation failed\n";
    free(strForFirstEx);
    return 1;
  }

  std::cout << los::lat_rmv(str, strForFirstEx, str_size) << "\n";
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
  size_t i = 0;
  for (; in && i < size; ++i) {
    in >> str[i];
    if (in.fail()) {
      break;
    }
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
  char * str_final = reinterpret_cast<char*>(malloc((i + 1) * sizeof(char)));
  if (!str_final) {
    std::cerr << "Memory allocation failed\n";
    free(str_final);
    free(str);
    return nullptr;
  }
  for (size_t j = 0; j < size; j++) {
    str_final[j] = str[j];
  }
  str = str_final;
  if (is_skips) {
    in >> std::skipws;
  }
  return str;
}

char * losev::lat_rmv(const char * src, char * dest, size_t size) {
  size_t j = 0;
  for (size_t i = 0; i < size; i++) {
    if (isalpha(src[i])) {
      continue;
    }
    else {
      dest[j++] = src[i];
    }
  }
  dest[j] = 0;
  return dest;
}