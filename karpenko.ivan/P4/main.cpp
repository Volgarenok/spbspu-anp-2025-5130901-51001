#include <iostream>
#include <cstring>
#include <cstddef>
#include "string_reader.hpp"
#include "array_utils.hpp"
#include "string_functions.hpp"

int main()
{
  size_t line1Length = 0;
  char* line1 = nullptr;

  try
  {
    line1 = karpenko::readStringWithAmortization(std::cin,
      line1Length, karpenko::isWordChar);
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Error: cannot allocate memory for input\n";
    return 1;
  }

  if (line1Length == 0 || line1[0] == '\0')
  {
    delete[] line1;
    std::cerr << "Error: empty input\n";
    return 1;
  }

  const char line2[] = "def_";
  const size_t line2Length = sizeof(line2) - 1;

  const size_t result1Size = line1Length + line2Length + 1;

  char* result1 = nullptr;
  char* result2 = nullptr;

  try
  {
    result1 = new char[result1Size];
    result1[result1Size - 1] = '\0';
    result2 = new char[karpenko::ALPHABET_RESULT_SIZE];
    result2[karpenko::ALPHABET_RESULT_SIZE - 1] = '\0';

    karpenko::uniTwo(line1, line1Length, line2, line2Length, result1);
    std::cout << result1 << '\n';

    karpenko::shrSym(line1, result2);
    std::cout << result2 << '\n';

    size_t wordCount = 0;
    std::cin.clear();
    std::cin.seekg(0);

    char** words = karpenko::readWords(std::cin, wordCount);

    if (words != nullptr && wordCount > 0)
    {
      for (size_t i = 0; i < wordCount; ++i)
      {
        karpenko::uniTwo(words[i], std::strlen(words[i]), line2, line2Length, result1);
        std::cout << result1 << '\n';

        karpenko::shrSym(words[i], result2);
        std::cout << result2 << '\n';
      }

      for (size_t i = 0; i < wordCount; ++i)
      {
        delete[] words[i];
      }
      delete[] words;
    }
    else if (words != nullptr)
    {
      delete[] words;
    }

    delete[] line1;
    delete[] result1;
    delete[] result2;
  }
  catch (const std::bad_alloc&)
  {
    delete[] line1;
    delete[] result1;
    delete[] result2;
    std::cerr << "Error: cannot allocate memory for results\n";
    return 1;
  }

  return 0;
}
