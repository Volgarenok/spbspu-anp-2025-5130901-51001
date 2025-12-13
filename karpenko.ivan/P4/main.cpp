#include <iostream>
#include <cstring>
#include <cctype>

namespace karpenko
{
  const size_t ALPHABET_SIZE = 26;
  const size_t ALPHABET_RESULT_SIZE = 27;

  void uniTwo(const char *str1, const char *str2, char *result, size_t resultSize)
  {
    const size_t len1 = std::strlen(str1);
    const size_t len2 = std::strlen(str2);
    const size_t maxLen = (len1 > len2) ? len1 : len2;

    size_t resultIndex = 0;

    for (size_t i = 0; i < maxLen; ++i)
    {
      if (i < len1 && resultIndex < resultSize - 1)
      {
        result[resultIndex++] = str1[i];
      }
      if (i < len2 && resultIndex < resultSize - 1)
      {
        result[resultIndex++] = str2[i];
      }
    }

    if (len1 > len2)
    {
      for (size_t i = len2; i < len1 && resultIndex < resultSize - 1; ++i)
      {
        result[resultIndex++] = str1[i];
      }
    }
    else if (len2 > len1)
    {
      for (size_t i = len1; i < len2 && resultIndex < resultSize - 1; ++i)
      {
        result[resultIndex++] = str2[i];
      }
    }

    result[resultIndex] = '\0';
  }

  void shrSym(const char *input, char *result, size_t resultSize)
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
    for (size_t i = 0; i < ALPHABET_SIZE && resultIndex < resultSize - 1; ++i)
    {
      if (!letters[i])
      {
        result[resultIndex++] = 'a' + i;
      }
    }
    result[resultIndex] = '\0';
  }

  char *myGetline()
  {
    const size_t INITIAL_BUFFER_SIZE = 16;
    const size_t GROW_FACTOR = 2;

    size_t bufferSize = INITIAL_BUFFER_SIZE;
    char *buffer = new char[bufferSize];

    if (!buffer)
    {
      return nullptr;
    }

    size_t i = 0;
    char c;

    while (std::cin.get(c) && c != '\n')
    {
      if (i >= bufferSize - 1)
      {
        size_t newSize = bufferSize * GROW_FACTOR;
        char *newBuffer = new char[newSize];

        if (!newBuffer)
        {
          delete[] buffer;
          return nullptr;
        }

        std::memcpy(newBuffer, buffer, i);
        delete[] buffer;
        buffer = newBuffer;
        bufferSize = newSize;
      }

      buffer[i++] = c;
    }

    buffer[i] = '\0';

    if (std::cin.eof() && i == 0)
    {
      delete[] buffer;
      return nullptr;
    }

    return buffer;
  }
}

int main()
{
  char *line1 = karpenko::myGetline();

  if (!line1)
  {
    std::cerr << "Error reading string\n";
    return 1;
  }

  const char line2[] = "def_";

  char *result1 = nullptr;
  char *result2 = nullptr;

  try
  {
    const size_t result1Size = std::strlen(line1) + std::strlen(line2);
    result1 = new char[result1Size + 1]();

    if (result1 == nullptr)
    {
      delete[] line1;
      std::cerr << "Error: cannot allocate memory for result\n";
      return 1;
    }

    karpenko::uniTwo(line1, line2, result1, result1Size + 1);
    std::cout << result1 << '\n';

    result2 = new char[karpenko::ALPHABET_RESULT_SIZE]();

    if (result2 == nullptr)
    {
      delete[] line1;
      delete[] result1;
      std::cerr << "Error: cannot allocate memory for result\n";
      return 1;
    }

    karpenko::shrSym(line1, result2, karpenko::ALPHABET_RESULT_SIZE);
    std::cout << result2 << '\n';

    delete[] line1;
    delete[] result1;
    delete[] result2;
  }
  catch (const std::bad_alloc &)
  {
    delete[] line1;
    if (result1 != nullptr)
    {
      delete[] result1;
    }
    if (result2 != nullptr)
    {
      delete[] result2;
    }
    std::cerr << "Error: cannot allocate memory for result\n";
    return 1;
  }
  catch (const std::exception &e)
  {
    delete[] line1;
    if (result1 != nullptr)
    {
      delete[] result1;
    }
    if (result2 != nullptr)
    {
      delete[] result2;
    }
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
