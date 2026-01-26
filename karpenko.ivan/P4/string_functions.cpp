#include "string_functions.hpp"
#include <cctype>
#include <cstddef>

const size_t karpenko::ALPHABET_SIZE = 26;
const size_t karpenko::ALPHABET_RESULT_SIZE = 27;

void karpenko::uniTwo(const char* str1, size_t len1, const char* str2, size_t len2,
  char* result)
{
  if (result == nullptr)
  {
    return;
  }

  size_t resultIndex = 0;

  for (size_t i = 0; i < len1 || i < len2; ++i)
  {
    if (i < len1)
    {
      result[resultIndex++] = str1[i];
    }
    if (i < len2)
    {
      result[resultIndex++] = str2[i];
    }
  }

  result[resultIndex] = '\0';
}

void karpenko::shrSym(const char* input, char* result)
{
  bool letters[ALPHABET_SIZE] = {false};

  for (size_t i = 0; input[i] != '\0'; ++i)
  {
    const unsigned char uc = static_cast< unsigned char >(input[i]);
    if (std::isalpha(uc))
    {
      const char lowerC = std::tolower(uc);
      const size_t index = lowerC - 'a';
      if (index < ALPHABET_SIZE)
      {
        letters[index] = true;
      }
    }
  }

  size_t resultIndex = 0;
  for (size_t i = 0; i < ALPHABET_SIZE; ++i)
  {
    if (!letters[i])
    {
      result[resultIndex++] = 'a' + i;
    }
  }
  result[resultIndex] = '\0';
}
