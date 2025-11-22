#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>

namespace karpenko
{
void uni_two(const char *str1, const char *str2, char *result)
{
  size_t len1 = strlen(str1);
  size_t len2 = strlen(str2);
  size_t max_len = (len1 > len2) ? len1 : len2;

  size_t result_index = 0;
  for (size_t i = 0; i < max_len; ++i)
  {
    if (i < len1)
    {
      result[result_index++] = str1[i];
    }
    if (i < len2)
    {
      result[result_index++] = str2[i];
    }
  }
  result[result_index] = '\0';
}
void shr_sym(const char *input, char *result)
{
  bool letters[26] = {false};

  for (size_t i = 0; input[i] != '\0'; ++i)
  {
    char c = input[i];
    if (std::isalpha(c))
    {
      char lower_c = std::tolower(c);
      letters[lower_c - 'a'] = true;
    }
  }

  size_t result_index = 0;
  for (int i = 0; i < 26; ++i)
  {
    if (!letters[i])
    {
      result[result_index++] = 'a' + i;
    }
  }
  result[result_index] = '\0';
}
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <1|2>" << std::endl;
    return 1;
  }

  int mode = std::atoi(argv[1]);
  if (mode != 1 && mode != 2)
  {
    std::cerr << "Invalid mode. Use 1 or 2" << std::endl;
    return 1;
  }

  std::ifstream input_file("input.txt");
  if (!input_file)
  {
    std::cerr << "Cannot open input.txt" << std::endl;
    return 1;
  }

  std::string line1, line2;

  if (mode == 1)
  {
    std::getline(input_file, line1);
    std::getline(input_file, line2);

    if (line1.empty() || line2.empty())
    {
      std::cerr << "Error: Need two non-empty lines" << std::endl;
      return 1;
    }
  }
  else
  {
    std::getline(input_file, line1);
  }

  input_file.close();

  std::ofstream output_file("output.txt");
  if (!output_file)
  {
    std::cerr << "Cannot open output.txt" << std::endl;
    return 1;
  }

  if (mode == 1)
  {
    size_t result_size = line1.length() + line2.length() + 1;
    char *result = new char[result_size];

    karpenko::uni_two(line1.c_str(), line2.c_str(), result);

    output_file << result << std::endl;

    delete[] result;

    std::cout << "UNI-TWO successful" << std::endl;
  }
  else
  {
    char *result = new char[27];
    karpenko::shr_sym(line1.c_str(), result);
    output_file << result << std::endl;
    delete[] result;

    std::cout << "SHR-SYM successful" << std::endl;
  }

  output_file.close();
  return 0;
}
