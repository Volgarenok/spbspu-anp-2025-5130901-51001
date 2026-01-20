#include <iostream>
#include <istream>
#include <cstring>

namespace krivoshapov
{
  bool isWhitespace(char c)
  {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
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

  char **readWords(std::istream &in, size_t &wordCount)
  {
    const size_t MAX_WORDS = 1000;
    const size_t MAX_WORD_LEN = 256;

    char **words = new char *[MAX_WORDS];
    for (size_t i = 0; i < MAX_WORDS; ++i)
    {
      words[i] = nullptr;
    }

    wordCount = 0;
    char buffer[MAX_WORD_LEN];
    size_t bufIdx = 0;
    char ch = 0;

    while (in.get(ch))
    {
      if (isWhitespace(ch))
      {
        // Конец слова
        if (bufIdx > 0)
        {
          buffer[bufIdx] = '\0';

          // Выделяем память только для нужного размера слова
          words[wordCount] = new char[bufIdx + 1];
          std::strcpy(words[wordCount], buffer);
          ++wordCount;
          bufIdx = 0;

          if (wordCount >= MAX_WORDS)
          {
            break;
          }
        }
      }
      else
      {
        // Символ слова
        if (bufIdx < MAX_WORD_LEN - 1)
        {
          buffer[bufIdx] = ch;
          ++bufIdx;
        }
      }
    }

    // Последнее слово, если есть
    if (bufIdx > 0 && wordCount < MAX_WORDS)
    {
      buffer[bufIdx] = '\0';
      words[wordCount] = new char[bufIdx + 1];
      std::strcpy(words[wordCount], buffer);
      ++wordCount;
    }

    if (in.bad())
    {
      for (size_t i = 0; i < wordCount; ++i)
      {
        delete[] words[i];
      }
      delete[] words;
      throw std::runtime_error("Input stream error");
    }

    return words;
  }

  // Применение функции проверки ко всем словам
  void processAllWords(char **words, size_t wordCount, int (*checkFunc)(const char *, size_t))
  {
    for (size_t i = 0; i < wordCount; ++i)
    {
      size_t len = std::strlen(words[i]);
      int result = checkFunc(words[i], len);
      std::cout << result;
      if (i < wordCount - 1)
      {
        std::cout << " ";
      }
    }
    std::cout << "\n";
  }

  // Освобождение памяти
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
    words = krivoshapov::readWords(std::cin, wordCount);
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

  if (wordCount == 0)
  {
    std::cout << "\n";
    krivoshapov::freeWords(words, wordCount);
    return 0;
  }

  // Применяем функцию проверки ко всем словам
  krivoshapov::processAllWords(words, wordCount, krivoshapov::seqSym);

  krivoshapov::freeWords(words, wordCount);
  return 0;
}
