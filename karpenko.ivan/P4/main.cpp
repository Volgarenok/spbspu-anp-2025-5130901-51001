#include <iostream>
#include <cstring>
#include <cctype>

namespace karpenko
{
  const size_t ALPHABET_SIZE = 26;
  const size_t ALPHABET_RESULT_SIZE = 27;
  const size_t INITIAL_CAPACITY = 16;
  const double GROW_FACTOR = 1.5;

  bool isWordChar(char ch)
  {
    return !std::isspace(static_cast< unsigned char >(ch));
  }

  char* readStringWithAmortization(std::istream& in, size_t& size,
    bool (*checkChar)(char))
  {
    size_t capacity = INITIAL_CAPACITY;
    char* buffer = new char[capacity];

    size = 0;
    char c;

    while (in.get(c) && checkChar(c))
    {
      if (size >= capacity - 1)
      {
        size_t newCapacity = static_cast< size_t >(capacity * GROW_FACTOR);
        if (newCapacity <= capacity)
        {
          newCapacity = capacity + 1;
        }

        char* newBuffer = nullptr;
        try
        {
          newBuffer = new char[newCapacity];
        }
        catch (const std::bad_alloc&)
        {
          delete[] buffer;
          throw;
        }

        std::memcpy(newBuffer, buffer, size);
        delete[] buffer;
        buffer = newBuffer;
        capacity = newCapacity;
      }

      buffer[size++] = c;
    }

    buffer[size] = '\0';
    return buffer;
  }

  char** readWords(std::istream& in, size_t& wordCount)
  {
    const size_t MAX_WORDS = 100;
    char** words = new char*[MAX_WORDS];

    wordCount = 0;
    char c;
    bool inWord = false;
    size_t wordSize = 0;
    char* currentWord = nullptr;

    auto finishWord = [&]()
    {
      if (inWord && wordSize > 0)
      {
        if (wordCount >= MAX_WORDS)
        {
          for (size_t i = 0; i < wordCount; ++i)
          {
            delete[] words[i];
          }
          delete[] words;
          std::cerr << "Error: too many words\n";
          std::exit(1);
        }
        words[wordCount] = currentWord;
        wordCount++;
        inWord = false;
        wordSize = 0;
        currentWord = nullptr;
      }
    };

    while (in.get(c))
    {
      if (c == '\n')
      {
        finishWord();
        break;
      }

      if (isWordChar(c))
      {
        if (!inWord)
        {
          inWord = true;
          currentWord = new char[INITIAL_CAPACITY];
          wordSize = 0;
        }

        if (wordSize >= INITIAL_CAPACITY - 1)
        {
          size_t newCapacity = static_cast< size_t >(INITIAL_CAPACITY * GROW_FACTOR);
          char* newBuffer = nullptr;
          try
          {
            newBuffer = new char[newCapacity];
          }
          catch (const std::bad_alloc&)
          {
            delete[] currentWord;
            throw;
          }

          std::memcpy(newBuffer, currentWord, wordSize);
          delete[] currentWord;
          currentWord = newBuffer;
        }

        currentWord[wordSize++] = c;
        currentWord[wordSize] = '\0';
      }
      else
      {
        finishWord();
      }
    }

    finishWord();

    if (wordCount == 0)
    {
      delete[] words;
      return nullptr;
    }

    char** resizedWords = new char*[wordCount];
    for (size_t i = 0; i < wordCount; ++i)
    {
      resizedWords[i] = words[i];
    }
    delete[] words;

    return resizedWords;
  }

  void uniTwo(const char* str1, size_t len1, const char* str2, size_t len2,
    char* result, size_t resultSize)
  {
    if (result == nullptr || resultSize == 0)
    {
      return;
    }

    const size_t maxLen = (len1 > len2) ? len1 : len2;
    size_t resultIndex = 0;

    for (size_t i = 0; i < maxLen; ++i)
    {
      if (i < len1 && resultIndex < resultSize - 1)
      {
        result[resultIndex++] = str1[i];
      }
      if (i < len2 && resultIndex < resultSize - 1)
      {
        result[resultIndex++] = str2[i];
      }
    }

    if (len1 > len2)
    {
      for (size_t i = len2; i < len1 && resultIndex < resultSize - 1; ++i)
      {
        result[resultIndex++] = str1[i];
      }
    }
    else if (len2 > len1)
    {
      for (size_t i = len1; i < len2 && resultIndex < resultSize - 1; ++i)
      {
        result[resultIndex++] = str2[i];
      }
    }

    result[resultIndex] = '\0';
  }

  void shrSym(const char* input, char* result)
  {
    bool letters[ALPHABET_SIZE] = {false};

    for (size_t i = 0; input[i] != '\0'; ++i)
    {
      const unsigned char uc = static_cast< unsigned char >(input[i]);
      if (std::isalpha(uc))
      {
        const char lowerC = std::tolower(uc);
        const size_t index = lowerC - 'a';
        if (index < ALPHABET_SIZE)
        {
          letters[index] = true;
        }
      }
    }

    size_t resultIndex = 0;
    for (size_t i = 0; i < ALPHABET_SIZE; ++i)
    {
      if (!letters[i])
      {
        result[resultIndex++] = 'a' + i;
      }
    }
    result[resultIndex] = '\0';
  }
}

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
    result2 = new char[karpenko::ALPHABET_RESULT_SIZE];

    karpenko::uniTwo(line1, line1Length, line2, line2Length,
      result1, result1Size);
    std::cout << result1 << '\n';

    karpenko::shrSym(line1, result2);
    std::cout << result2 << '\n';

    size_t wordCount = 0;
    std::cin.clear();
    std::cin.seekg(0);

    char** words = karpenko::readWords(std::cin, wordCount);

    if (words != nullptr)
    {
      for (size_t i = 0; i < wordCount; ++i)
      {
        karpenko::uniTwo(words[i], std::strlen(words[i]), line2,
          line2Length, result1, result1Size);
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
