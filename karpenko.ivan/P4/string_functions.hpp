#ifndef STRING_FUNCTIONS_HPP
#define STRING_FUNCTIONS_HPP

namespace karpenko
{
  extern const size_t ALPHABET_SIZE;
  extern const size_t ALPHABET_RESULT_SIZE;

  void uniTwo(const char* str1, size_t len1, const char* str2, size_t len2,
    char* result, size_t resultSize);

  void shrSym(const char* input, char* result);
}

#endif
