#include <iostream>
#include <istream>
#include <cstring>
#include <cctype>

namespace krivoshapov
{
  bool isDelimiter(char c)
  {
    return std::isspace(static_cast<unsigned char>(c));
  }

  int seqSym(const char *str, size_t len)
  {
    if (str == nullptr || len < 2)
    {
      return 0;
    }

    for (size_t i = 0; i + 1 < len; ++i)
    {
      if (str[i] == str[i + 1])
      {
        return 1;
      }
    }
    return 0;
  }

  char *readToken(std::istream &in, bool (*isDelim)(char))
  {
    size_t capacity = 16;
    size_t size = 0;
    char *buffer = new char[capacity];

    char ch;
    while (in.get(ch))
    {
      if (isDelim(ch))
      {
        if (size > 0)
        {
          break;
        }
        else
        {
          continue;
        }
      }

      if (size + 1 >= capacity)
      {
        capacity *= 2;
        char *temp = new char[capacity];
        std::memcpy(temp, buffer, size);
        delete[] buffer;
        buffer = temp;
      }

      buffer[size++] = ch;
    }

    if (size == 0)
    {
      delete[] buffer;
      return nullptr;
    }

    buffer[size] = '\0';
    return buffer;
  }

  char **readWords(std::istream &in, size_t &wordCount, bool (*isDelim)(char))
  {
    size_t capacity = 8;
    wordCount = 0;
    char **words = new char *[capacity];

    while (true)
    {
      char *word = readToken(in, isDelim);
      if (word == nullptr)
      {
        break;
      }

      if (wordCount == capacity)
      {
        capacity *= 2;
        char **temp = new char *[capacity];
        for (size_t i = 0; i < wordCount; ++i)
        {
          temp[i] = words[i];
        }
        delete[] words;
        words = temp;
      }

      words[wordCount++] = word;
    }

    return words;
  }

  void freeWords(char **words, size_t wordCount)
  {
    if (words == nullptr)
    {
      return;
    }

    for (size_t i = 0; i < wordCount; ++i)
    {
      delete[] words[i];
    }
    delete[] words;
  }
}

int main()
{
  size_t wordCount = 0;
  char **words = nullptr;

  try
  {
    words = krivoshapov::readWords(
        std::cin,
        wordCount,
        krivoshapov::isDelimiter);
  }
  catch (const std::bad_alloc &)
  {
    std::cerr << "Memory allocation failed\n";
    return 1;
  }

  if (wordCount == 0)
  {
    krivoshapov::freeWords(words, wordCount);
    std::cerr << "Empty input\n";
    return 2;
  }

  for (size_t i = 0; i < wordCount; ++i)
  {
    size_t len = std::strlen(words[i]);
    int res = krivoshapov::seqSym(words[i], len);
    std::cout << res;
    if (i + 1 < wordCount)
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";

  krivoshapov::freeWords(words, wordCount);
  return 0;
}
