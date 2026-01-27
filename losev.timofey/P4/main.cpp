#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<cctype>

namespace losev {
  char * getline(std::istream & in, size_t & capacity, size_t & actual_size);
  char * latRmv(const char * src, char * dest);
  int hasRep(const char * src);
}

int main() {
  namespace los = losev;
  size_t str_size = 10;
  size_t actual_len = 0;
  char * str = los::getline(std::cin, str_size, actual_len);
  if (str == nullptr) {
    std::cerr << "Fail memory\n";
    return 1;
  }
  if (str[0] == '\0') {
    std::cerr << "Empty input\n";
    free(str);
    return 1;
  }
  char * strForFirstEx = reinterpret_cast< char* >(malloc(str_size * sizeof(char)));
  if (!strForFirstEx) {
    std::cerr << "Memory allocation failed\n";
    free(str);
    return 1;
  }
  strForFirstEx[0] = '\0';
  std::cout << los::latRmv(str, strForFirstEx) << "\n" << los::hasRep(str) << "\n";
  free(str);
  free(strForFirstEx);
  return 0;
}

char * losev::getline(std::istream & in, size_t & capacity, size_t & actual_size) {
  capacity = 10;
  char * str = reinterpret_cast< char* >(malloc(capacity * sizeof(char)));
  if (!str) {
    return nullptr;
  }
  actual_size = 0;
  bool is_skips = in.flags() & std::ios_base::skipws;
  if (is_skips) {
    in >> std::noskipws;
  }
  while (in) {
    if (actual_size >= (capacity - 1)) {
      size_t new_capacity = capacity + 10;
      char * str_new = reinterpret_cast< char* >(malloc(new_capacity * sizeof(char)));
      if (!str_new) {
        if (is_skips) {
          in >> std::skipws;
        }
        free(str);
        return nullptr;
      }
      for (size_t j = 0; j < actual_size; j++) {
        str_new[j] = str[j];
      }
      free(str);
      str = str_new;
      capacity = new_capacity;
    }
    in >> str[actual_size];
    if (in.fail()) {
      break;
    }
    actual_size++;
  }
  str[actual_size] = '\0';
  if (is_skips) {
    in >> std::skipws;
  }
  return str;
}

char * losev::latRmv(const char * src, char * dest) {
  size_t j = 0;
  for (size_t i = 0; src[i] != '\0'; i++) {
    if (!isalpha(src[i])) {
      dest[j++] = src[i];
    }
  }
  dest[j] = '\0';
  return dest;
}

int losev::hasRep(const char * src) {
  const char * ptr = src + 1;
  for (; *src != '\0'; src++) {
    ptr = src + 1;
    for (; *ptr != '\0'; ptr++) {
      if (*ptr == *src) {
        return 1;
      }
    }
  }
  return 0;
}
