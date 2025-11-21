#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<cctype>

namespace losev {
  char * getline(std::istream & in, size_t & size);
  char * lat_rmv(const char * src, char * dest, size_t size);
  int has_rep(const char * src, size_t size);
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
    free(str);
    return 1;
  }
  for (size_t i = 0; i < str_size; i++) {
    strForFirstEx[i] = '\0';
  }
  std::cout << los::lat_rmv(str, strForFirstEx, str_size) << "\n" << los::has_rep(str, str_size) << "\n";
  free(str);
  free(strForFirstEx);
  return 0;
}

char * losev::getline(std::istream & in, size_t & size) {
  char * str = reinterpret_cast<char*>(malloc(size * sizeof(char)));
  if (!str) {
    std::cerr << "Memory allocation failed\n";
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
    if (i == (size - 1)) {
      char * str_new = reinterpret_cast<char*>(malloc((size + 10) * sizeof(char)));
      if (!str_new) {
        std::cerr << "Memory allocation failed\n";
        free(str);
        return nullptr;
      }
      for (size_t j = 0; j < size; j++) {
        str_new[j] = str[j];
      }
      free(str);
      size += 10;
      str = str_new;
    }
  }
  size = i + 1;
  char * str_final = reinterpret_cast<char*>(malloc((size) * sizeof(char)));
  if (!str_final) {
    std::cerr << "Memory allocation failed\n";
    free(str);
    return nullptr;
  }
  for (size_t j = 0; j < size; j++) {
    str_final[j] = str[j];
  }
  free(str);
  str = str_final;
  if (i == 0) {
    free(str);
    if (is_skips) {
      in >> std::skipws;
    }
    return nullptr;
  }
  str[size - 1] = '\0';
  if (is_skips) {
    in >> std::skipws;
  }
  return str;
}

char * losev::lat_rmv(const char * src, char * dest, size_t size) {
  size_t j = 0;
  for (size_t i = 0; i < size && src[i] != '\0'; i++) {
    if (!isalpha(src[i])) {
      if (j < size - 1) {
        dest[j++] = src[i];
      }
    }
  }
  dest[j] = '\0';
  return dest;
}

int losev::has_rep(const char * src, size_t size) {
  if (size < 2) {
    return 0;
  }
  const char * ptr = src + 1;
  for (size_t i = 0; i < size - 1 && *src != '\0'; i++, src++) {
    ptr = src + 1;
    for (size_t j = 0; j < size - 1 && *ptr != '\0'; j++, ptr++) {
      if (*ptr == *src) {
        return 1;
      }
    }
  }
  return 0;
}
