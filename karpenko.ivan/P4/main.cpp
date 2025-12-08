#include <iostream>
#include <cstring>
#include <cctype>

namespace karpenko
{
  void uni_two(const char *str1, const char *str2, char *result, size_t result_size)
  {
    size_t len1 = std::strlen(str1);
    size_t len2 = std::strlen(str2);
    size_t max_len = (len1 > len2) ? len1 : len2;

    size_t result_index = 0;
    for (size_t i = 0; i < max_len && result_index < result_size - 1; ++i)
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
    result[result_index] = '\0';
  }

  void shr_sym(const char *input, char *result, size_t result_size)
  {
    bool letters[26] = {false};

    for (size_t i = 0; input[i] != '\0'; ++i)
    {
      char c = input[i];
      if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
      {
        char lower_c = std::tolower(static_cast<unsigned char>(c));
        size_t index = lower_c - 'a';
        if (index < 26)
        {
          letters[index] = true;
        }
      }
    }

    size_t result_index = 0;
    for (int i = 0; i < 26 && result_index < result_size - 1; ++i)
    {
      if (!letters[i])
      {
        result[result_index++] = 'a' + i;
      }
    }
    result[result_index] = '\0';
  }
}

int main()
{
  std::string line1, line2;
  if (!std::getline(std::cin, line1))
  {
    std::cerr << "Error reading first string" << "\n";
    return 1;
  }
  if (!std::getline(std::cin, line2))
  {
    line2 = "";
  }
  try
  {
    size_t result1_size = line1.length() + line2.length() + 1;
    char *result1 = new char[result1_size];
    karpenko::uni_two(line1.c_str(), line2.c_str(), result1, result1_size);
    std::cout << result1 << "\n";

    char *result2 = new char[27];
    karpenko::shr_sym(line1.c_str(), result2, 27);
    std::cout << result2 << "\n";

    char *result3 = new char[27];
    karpenko::shr_sym(line2.c_str(), result3, 27);
    std::cout << result3 << "\n";

    delete[] result1;
    delete[] result2;
    delete[] result3;
  }
  catch (const std::bad_alloc &)
  {
    std::cerr << "Error: cannot allocate memory for result" << "\n";
    return 1;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
