#include "string_reader.hpp"
#include "array_utils.hpp"
#include "string_functions.hpp"
#include <iostream>
#include <cstring>
#include <cstddef>

int main()
{
  size_t line1_length = 0;
  char* line1 = nullptr;

  try
  {
    line1 = karpenko::read_string_with_amortization(std::cin,
      line1_length, karpenko::is_word_char);
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Error: cannot allocate memory for input\n";
    return 1;
  }

  if (line1_length == 0 || line1[0] == '\0')
  {
    delete[] line1;
    std::cerr << "Error: empty input\n";
    return 1;
  }

  const char line2[] = "def_";
  const size_t line2_length = sizeof(line2) - 1;

  const size_t result1_size = line1_length + line2_length + 1;

  char* result1 = nullptr;
  char* result2 = nullptr;

  try
  {
    result1 = new char[result1_size];
    result2 = new char[karpenko::ALPHABET_RESULT_SIZE];

    karpenko::uni_two(line1, line1_length, line2, line2_length,
      result1, result1_size);
    std::cout << result1 << '\n';

    karpenko::shr_sym(line1, result2);
    std::cout << result2 << '\n';

    size_t word_count = 0;
    std::cin.clear();
    std::cin.seekg(0);

    char** words = karpenko::read_words(std::cin, word_count);

    if (words != nullptr)
    {
      for (size_t i = 0; i < word_count; ++i)
      {
        karpenko::uni_two(words[i], std::strlen(words[i]), line2,
          line2_length, result1, result1_size);
        std::cout << result1 << '\n';

        karpenko::shr_sym(words[i], result2);
        std::cout << result2 << '\n';
      }

      for (size_t i = 0; i < word_count; ++i)
      {
        delete[] words[i];
      }
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
  catch (...)
  {
    delete[] line1;
    delete[] result1;
    delete[] result2;
    std::cerr << "Error: unexpected exception\n";
    return 1;
  }

  return 0;
}
