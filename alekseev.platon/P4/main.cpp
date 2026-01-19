#include <iostream>
#include <stdexcept>
#include <new>

namespace alekseev
{
  int containsChar(const char* s, char c)
  {
    for (; *s != '\0'; ++s)
    {
      if (*s == c)
      {
        return 1;
      }
    }
    return 0;
  }

  char* getline(std::istream& in, size_t& size)
  {
    const std::ios_base::fmtflags oldFlags = in.flags();
    const bool hadSkipws = (oldFlags & std::ios_base::skipws) != 0;

    if (hadSkipws)
    {
      in >> std::noskipws;
    }

    char* buf = nullptr;

    try
    {
      size_t cap = 16;
      buf = new char[cap];
      size = 0;

      char c = '\0';
      while ((in >> c) && c != '\n')
      {
        if (size + 1 >= cap)
        {
          const size_t newCap = cap * 2;
          char* newBuf = new char[newCap];

          for (size_t i = 0; i < size; ++i)
          {
            newBuf[i] = buf[i];
          }

          delete[] buf;
          buf = newBuf;
          cap = newCap;
        }
        buf[size++] = c;
      }

      if (in.fail() && !in.eof())
      {
        delete[] buf;
        in.flags(oldFlags);
        size = 0;
        throw std::logic_error("input failed");
      }

      buf[size] = '\0';
      in.flags(oldFlags);
      return buf;
    }
    catch (const std::bad_alloc&)
    {
      delete[] buf;
      in.flags(oldFlags);
      size = 0;
      throw;
    }
  }

  size_t removeVowels(const char* s, char* d)
  {
    const char* vowels = "aeiouAEIOU";
    size_t i = 0;

    for (; *s != '\0'; ++s)
    {
      const char c = *s;
      if (!containsChar(vowels, c))
      {
        d[i++] = c;
      }
    }

    d[i] = '\0';
    return i;
  }

  size_t excludeChars(const char* s, const char* exc, char* d)
  {
    size_t i = 0;

    for (; *s != '\0'; ++s)
    {
      const char c = *s;
      if (!containsChar(exc, c))
      {
        d[i++] = c;
      }
    }

    d[i] = '\0';
    return i;
  }

}

int main()
{
  using namespace alekseev;

  size_t size1 = 0;
  char* input1 = nullptr;

  try
  {
    input1 = getline(std::cin, size1);
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Error: cannot allocate memory\n";
    return 1;
  }
  catch (const std::logic_error& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }

  if (size1 == 0)
  {
    std::cerr << "Error: empty input\n";
    delete[] input1;
    return 1;
  }

  const char* second = "abc";

  char* result1 = nullptr;
  char* result2 = nullptr;

  try
  {
    result1 = new char[size1 + 1];
    result1[size1] = '\0';

    result2 = new char[size1 + 1];
    result2[size1] = '\0';
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Error: cannot allocate memory\n";
    delete[] result1;
    delete[] result2;
    delete[] input1;
    return 1;
  }

  removeVowels(input1, result1);
  std::cout << result1 << "\n";

  excludeChars(input1, second, result2);
  std::cout << result2 << "\n";

  delete[] result2;
  delete[] result1;
  delete[] input1;
  return 0;
}
