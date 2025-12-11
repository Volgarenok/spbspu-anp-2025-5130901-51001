#include <iostream>
#include <cstring>
#include <cctype>

namespace karpenko
{
  const size_t ALPHABET_SIZE = 26;
  const size_t ALPHABET_RESULT_SIZE = 27;

  void uniTwo(const char * str1, const char * str2, char * result, size_t resultSize)
  {
    const size_t len1 = std::strlen(str1);
    const size_t len2 = std::strlen(str2);
    const size_t maxLen = (len1 > len2) ? len1 : len2;

    size_t resultIndex = 0;
    for (size_t i = 0; i < maxLen && resultIndex < resultSize - 1; ++i)
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
    result[resultIndex] = '\0';
  }

  void shrSym(const char * input, char * result, size_t resultSize)
  {
    bool letters[ALPHABET_SIZE] = {false};

    for (size_t i = 0; input[i] != '\0'; ++i)
    {
      const unsigned char uc = static_cast<unsigned char>(input[i]);
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
}

bool myGetline(char * buffer, size_t bufferSize)
{
  if (bufferSize == 0)
  {
    return false;
  }

  size_t i = 0;
  char c;

  while (std::cin.get(c) && c != '\n')
  {
    if (i < bufferSize - 1)
    {
      buffer[i++] = c;
    }
  }

  buffer[i] = '\0';

  return !(std::cin.eof() && i == 0);
}

int main()
{
  const size_t MAX_LINE_SIZE = 1000;
  char line1[MAX_LINE_SIZE] = {'\0'};
  char line2[MAX_LINE_SIZE] = {'\0'};

  if (!myGetline(line1, MAX_LINE_SIZE))
  {
    std::cerr << "Error reading first string\n";
    return 1;
  }

  if (!myGetline(line2, MAX_LINE_SIZE))
  {
    line2[0] = '\0';
  }

  try
  {
    const size_t result1Size = std::strlen(line1) + std::strlen(line2) + 1;
    char * result1 = new char[result1Size];
    karpenko::uniTwo(line1, line2, result1, result1Size);
    std::cout << result1 << '\n';
    delete[] result1;

    char * result2 = new char[karpenko::ALPHABET_RESULT_SIZE];
    karpenko::shrSym(line1, result2, karpenko::ALPHABET_RESULT_SIZE);
    std::cout << result2 << '\n';
    delete[] result2;

    char * result3 = new char[karpenko::ALPHABET_RESULT_SIZE];
    karpenko::shrSym(line2, result3, karpenko::ALPHABET_RESULT_SIZE);
    std::cout << result3 << '\n';
    delete[] result3;
  }
  catch (const std::bad_alloc &)
  {
    std::cerr << "Error: cannot allocate memory for result\n";
    return 2;
  }
  catch (const std::exception & e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 2;
  }

  return 0;
}
