#include <iostream>
#include <cctype>
#include <cstring>

namespace smirnova
{
  void identical_str(const char *a, const char *b, int *result, int n, int m)
  {
    for (size_t i = 0; i < n; i++)
    {
      for (size_t j = 0; j < m; j++)
      {
        if (a[i] == b[j])
        {
          *result = 1;
        }
      }
    }
  }

  void concatenation_str(const char *a, const char *b, char *result, int n, int m)
  {
    size_t count = 0;
    for (size_t i = 0; i < n; i++)
    {
      result[count++] = a[i];

      for (size_t j = 0; j < n; j++)
      {
        result[count++] = a[i];
      }

      for (size_t j = 0; j < m; j++)
      {
        if (std::isdigit(b[j]))
        {
          for (size_t k = 0; k < n; k++)
          {
            if (a[k] == b[j]) {
              result[count++] = b[j];
              break;
            }
          }
        }
      }
    }
    result[count] = '\0';
  }
}

int main()
{
  std::string l1, l2;

  if (!std::getline(std::cin, l1)) {
    std::cerr << "Couldn't read the first line\n";
    return 1;
  }

  if (!std::getline(std::cin, l2)) {
    l2 = "Hello, user from terminal\n";
  }

  size_t n = l1.length();
  size_t m = l2.length();

  try
  {
    size_t length = l1.length() + l2.length() + 1;
    int result1 = 0;
    char *result2 = new char[length];

    smirnova::identical_str(l1.c_str(), l2.c_str(), &result1, n, m);
    std::cout << result1 << "\n";

    smirnova::concatenation_str(l1.c_str(), l2.c_str(), result2, n, m);
    std::cout << result2 << "\n";

    delete[] result2;
  }
  catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
