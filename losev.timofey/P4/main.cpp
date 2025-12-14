#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<cctype>

namespace losev {
  char * getline(std::istream & in, size_t & size);
  char * latRmv(const char * src, char * dest, size_t size);
  int hasRep(const char * src, size_t size);
}

int main() {
  namespace los = losev;
  size_t str_size = 10;
  char * str =  los::getline(std::cin, str_size);
  if (str == nullptr) {
    std::cerr << "Fail memory\n";
    return 1;
  }
  char * strForFirstEx = reinterpret_cast< char* >(malloc(str_size * sizeof(char)));
  if (!strForFirstEx) {
    std::cerr << "Memory allocation failed\n";
    free(str);
    return 1;
  }
  std::cout << los::latRmv(str, strForFirstEx, str_size) << "\n" << los::hasRep(str, str_size) << "\n";
  free(str);
  free(strForFirstEx);
  return 0;
}

char * losev::getline(std::istream & in, size_t & capacity) {
  capacity = 10;
  char * str = reinterpret_cast< char* >(malloc(capacity * sizeof(char)));
  if (!str) {
    return nullptr;
  }
  bool is_skips = in.flags() & std::ios_base::skipws;
  if (is_skips) {
    in >> std::noskipws;
  }
  size_t size = 0;
  for (; in && size < capacity; ++size) {
    in >> str[size];
    if (in.fail()) {
      if (is_skips) {
        in >> std::skipws;
      }
      free(str);
      return nullptr;
    }
    if (size == (capacity - 1)) {
      capacity += 10;
      char * str_new = reinterpret_cast< char* >(malloc(capacity * sizeof(char)));
      if (!str_new) {
        if (is_skips) {
          in >> std::skipws;
        }
        free(str);
        return nullptr;
      }
      for (size_t j = 0; j < size; j++) {
        str_new[j] = str[j];
      }
      free(str);
      str = str_new;
    }
  }
  if (size == 0) {
    free(str);
    if (is_skips) {
      in >> std::skipws;
    }
    return nullptr;
  }
  str[size] = '\0';
  if (is_skips) {
    in >> std::skipws;
  }
  return str;
}

char * losev::latRmv(const char * src, char * dest, size_t size) {
  size_t j = 0;
  for (size_t i = 0; src[i] != '\0' && j < size - 1; i++) {
    if (!isalpha(src[i])) {
      dest[j++] = src[i];
    }
  }
  dest[j] = '\0';
  return dest;
}

int losev::hasRep(const char * src, size_t size) {
  if (size < 2) {
    return 0;
  }
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
