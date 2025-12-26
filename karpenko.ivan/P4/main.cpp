#include <iostream>
#include <cstring>
#include <cctype>

namespace karpenko
{
  const size_t ALPHABET_SIZE = 26;
  const size_t ALPHABET_RESULT_SIZE = 27;

  void uniTwo(const char *str1, size_t len1, const char *str2, size_t len2, char *result, size_t resultSize)
  {
    const size_t maxLen = (len1 > len2) ? len1 : len2;

    size_t resultIndex = 0;

    for (size_t i = 0; i < maxLen; ++i)
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

    if (len1 > len2)
    {
      for (size_t i = len2; i < len1; ++i)
      {
        result[resultIndex++] = str1[i];
      }
    }
    else if (len2 > len1)
    {
      for (size_t i = len1; i < len2; ++i)
      {
        result[resultIndex++] = str2[i];
      }
    }

    result[resultIndex] = '\0';
  }

  void shrSym(const char *input, char *result)
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

  char *myGetline(std::istream &in, size_t &size)
  {
    const size_t INITIAL_CAPACITY = 16;
    const size_t GROW_FACTOR = 2;

    size_t capacity = INITIAL_CAPACITY;
    char *buffer = new char[capacity];

    size = 0;
    char c;

    while (in.get(c) && c != '\n')
    {
      if (size >= capacity - 1)
      {
        size_t newCapacity = capacity * GROW_FACTOR;
        char *newBuffer = nullptr;

        try
        {
          newBuffer = new char[newCapacity];
        }
        catch (const std::bad_alloc &)
        {
          delete[] buffer;
          throw;
        }
        
        std::memcpy(newBuffer, buffer, size);
        delete[] buffer;
        buffer = newBuffer;
        capacity = newCapacity;
      }

      buffer[size++] = c;
    }

    buffer[size] = '\0';
    return buffer;
  }
}

int main()
{
  size_t line1Length = 0;
  char *line1 = karpenko::myGetline(std::cin, line1Length);

  if (!line1 || line1Length == 0)
  {
    delete[] line1;
    return 0;
  }

  const char line2[] = "def_";
  const size_t line2Length = sizeof(line2) - 1;

  const size_t result1Size = line1Length + line2Length;

  char *result1 = nullptr;
  char *result2 = nullptr;

  try
  {
    result1 = new char[result1Size + 1];
    result1[result1Size] = '\0';
    
    karpenko::uniTwo(line1, line1Length, line2, line2Length, result1, result1Size + 1);
    std::cout << result1 << '\n';

    result2 = new char[karpenko::ALPHABET_RESULT_SIZE];
    result2[karpenko::ALPHABET_RESULT_SIZE - 1] = '\0';
    
    karpenko::shrSym(line1, result2);
    std::cout << result2 << '\n';

    delete[] line1;
    delete[] result1;
    delete[] result2;
  }
  catch (const std::bad_alloc &)
  {
    delete[] line1;
    delete[] result1;
    delete[] result2;
    std::cerr << "Error: cannot allocate memory for result\n";
    return 1;
  }

  return 0;
}
