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
      if (std::isalpha(c))
      {
        char lower_c = std::tolower(static_cast<unsigned char>(c));
        letters[lower_c - 'a'] = true;
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
  std::string line1;
  std::cout << "Enter first string: ";
  if (!std::getline(std::cin, line1))
  {
    std::cerr << "Error reading first string" << std::endl;
    return 1;
  }

  std::string line2;
  std::cout << "Enter second string: ";
  if (!std::getline(std::cin, line2))
  {
    std::cerr << "Error reading second string" << std::endl;
    return 1;
  }

  size_t max_required_size = line1.length() + line2.length() + 1;
  const size_t MAX_ALLOWED_SIZE = 1000000;

  if (max_required_size > MAX_ALLOWED_SIZE)
  {
    std::cerr << "Error: result would be too large for memory" << std::endl;
    return 1;
  }

  try
  {
    char *result1 = new char[line1.length() + line2.length() + 1];
    karpenko::uni_two(line1.c_str(), line2.c_str(), result1,
                      line1.length() + line2.length() + 1);
    std::cout << "Combined string: " << result1 << std::endl;

    char *result2 = new char[27];
    karpenko::shr_sym(line1.c_str(), result2, 27);
    std::cout << "Missing letters in first string: " << result2 << std::endl;

    char *result3 = new char[27];
    karpenko::shr_sym(line2.c_str(), result3, 27);
    std::cout << "Missing letters in second string: " << result3 << std::endl;

    delete[] result1;
    delete[] result2;
    delete[] result3;
  }
  catch (const std::bad_alloc &)
  {
    std::cerr << "Error: cannot allocate memory for result" << std::endl;
    return 1;
  }

  return 0;
}
