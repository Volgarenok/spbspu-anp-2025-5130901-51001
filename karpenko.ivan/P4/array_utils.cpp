#include "array_utils.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

namespace karpenko
{
  bool isWordChar(char ch)
  {
    return !std::isspace(static_cast< unsigned char >(ch));
  }

  char** readWords(std::istream& in, size_t& wordCount)
  {
    const size_t MAX_WORDS = 100;
    const size_t INITIAL_CAPACITY = 16;
    const double GROW_FACTOR = 1.5;

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

  void resizeCharArray(char*& array, size_t& capacity, size_t requiredSize)
  {
    const double GROW_FACTOR = 1.5;
    size_t newCapacity = static_cast< size_t >(capacity * GROW_FACTOR);
    
    if (newCapacity <= capacity)
    {
      newCapacity = capacity + 1;
    }
    
    while (newCapacity < requiredSize)
    {
      newCapacity = static_cast< size_t >(newCapacity * GROW_FACTOR);
    }

    char* newArray = nullptr;
    try
    {
      newArray = new char[newCapacity];
    }
    catch (const std::bad_alloc&)
    {
      delete[] array;
      throw;
    }

    std::memcpy(newArray, array, capacity);
    delete[] array;
    array = newArray;
    capacity = newCapacity;
  }
}
