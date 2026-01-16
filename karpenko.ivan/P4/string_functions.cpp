#include "string_functions.hpp"
#include <cctype>
#include <cstddef>

namespace karpenko
{
  const size_t ALPHABET_SIZE = 26;
  const size_t ALPHABET_RESULT_SIZE = 27;

  void uni_two(const char* str1, size_t len1, const char* str2, size_t len2,
    char* result, size_t result_size)
  {
    if (result == nullptr || result_size == 0)
    {
      return;
    }

    const size_t max_len = (len1 > len2) ? len1 : len2;
    size_t result_index = 0;

    for (size_t i = 0; i < max_len; ++i)
    {
      if (i < len1 && result_index < result_size - 1)
      {
        result[result_index++] = str1[i];
      }
      if (i < len2 && result_index < result_size - 1)
      {
        result[result_index++] = str2[i];
      }
    }

    if (len1 > len2)
    {
      for (size_t i = len2; i < len1 && result_index < result_size - 1; ++i)
      {
        result[result_index++] = str1[i];
      }
    }
    else if (len2 > len1)
    {
      for (size_t i = len1; i < len2 && result_index < result_size - 1; ++i)
      {
        result[result_index++] = str2[i];
      }
    }

    result[result_index] = '\0';
  }

  void shr_sym(const char* input, char* result)
  {
    bool letters[ALPHABET_SIZE] = {false};

    for (size_t i = 0; input[i] != '\0'; ++i)
    {
      const unsigned char uc = static_cast< unsigned char >(input[i]);
      if (std::isalpha(uc))
      {
        const char lower_c = std::tolower(uc);
        const size_t index = lower_c - 'a';
        if (index < ALPHABET_SIZE)
        {
          letters[index] = true;
        }
      }
    }

    size_t result_index = 0;
    for (size_t i = 0; i < ALPHABET_SIZE; ++i)
    {
      if (!letters[i])
      {
        result[result_index++] = 'a' + i;
      }
    }
    result[result_index] = '\0';
  }
}
