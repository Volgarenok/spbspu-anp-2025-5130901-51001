#ifndef STRING_FUNCTIONS_HPP
#define STRING_FUNCTIONS_HPP

#include <cstddef>

namespace karpenko
{
  extern const size_t ALPHABET_SIZE;
  extern const size_t ALPHABET_RESULT_SIZE;

  void uni_two(const char* str1, size_t len1, const char* str2, size_t len2,
    char* result, size_t result_size);

  void shr_sym(const char* input, char* result);
}

#endif
