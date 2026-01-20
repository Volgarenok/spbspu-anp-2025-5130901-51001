#include <iostream>
#include <istream>
#include <cstring>

namespace krivoshapov
{
  char *rmvVow(const char *src, char *dst, size_t dstSz)
  {
    if (src == nullptr || dst == nullptr || dstSz == 0)
    {
      if (dst != nullptr && dstSz > 0)
      {
        dst[0] = '\0';
      }
      return dst;
    }

    const char vowels[] = "aeiouyAEIOUY";
    size_t writeIdx = 0;

    for (size_t readIdx = 0; src[readIdx] != '\0'; ++readIdx)
    {
      int isVowel = 0;
      char currentChar = src[readIdx];

      for (size_t i = 0; vowels[i] != '\0'; ++i)
      {
        if (currentChar == vowels[i])
        {
          isVowel = 1;
          break;
        }
      }

      if (!isVowel)
      {
        dst[writeIdx] = currentChar;
        ++writeIdx;
      }
    }

    dst[writeIdx] = '\0';
    return dst;
  }

  int seqSym(const char *str, size_t len)
  {
    if (str == nullptr || len < 2)
    {
      return 0;
    }

    for (size_t i = 0; i < len - 1; ++i)
    {
      if (str[i] == str[i + 1])
      {
        return 1;
      }
    }

    return 0;
  }

  char *readString(std::istream &in, size_t &size, size_t &capacity)
  {
    const size_t INIT_CAPACITY = 128;
    char *buffer = nullptr;

    buffer = new char[INIT_CAPACITY]();

    capacity = INIT_CAPACITY;
    size = 0;
    char ch = 0;

    while (in.get(ch) && ch != '\n')
    {
      if (size + 1 >= capacity)
      {
        size_t newCapacity = capacity * 2;
        char *newBuf = nullptr;

        newBuf = new char[newCapacity]();
        std::memcpy(newBuf, buffer, size);
        delete[] buffer;
        buffer = newBuf;
        capacity = newCapacity;
      }

      buffer[size] = ch;
      ++size;
    }

    if (in.bad())
    {
      delete[] buffer;
      throw std::runtime_error("Input stream error");
    }

    buffer[size] = '\0';
    return buffer;
  }
}

int main()
{
  size_t size = 0;
  size_t capacity = 0;

  char *inBuf = nullptr;
  try
  {
    inBuf = krivoshapov::readString(std::cin, size, capacity);
  }
  catch (const std::bad_alloc &)
  {
    std::cerr << "Memory allocation failed\n";
    return 1;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  char *resBuf = nullptr;
  try
  {
    resBuf = new char[capacity]();
  }
  catch (const std::bad_alloc &)
  {
    delete[] inBuf;
    std::cerr << "Memory allocation failed\n";
    return 1;
  }

  krivoshapov::rmvVow(inBuf, resBuf, capacity);
  std::cout << resBuf << "\n";

  int seqRes = krivoshapov::seqSym(inBuf, size);
  std::cout << seqRes << "\n";

  delete[] inBuf;
  delete[] resBuf;
  return 0;
}
