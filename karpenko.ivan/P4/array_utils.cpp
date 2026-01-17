#include "array_utils.hpp"
#include "string_reader.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

bool karpenko::isWordChar(char ch)
{
  return !std::isspace(static_cast< unsigned char >(ch));
}

bool karpenko::isSpaceChar(char ch)
{
  return std::isspace(static_cast< unsigned char >(ch));
}

char** karpenko::readWords(std::istream& in, size_t& wordCount)
{
  const size_t INITIAL_CAPACITY = 16;
  const double GROW_FACTOR = 1.5;

  size_t capacity = INITIAL_CAPACITY;
  char** words = new char*[capacity];
  wordCount = 0;

  size_t lineLength = 0;
  char* line = karpenko::readStringWithAmortization(in, lineLength, karpenko::isWordChar);

  if (line == nullptr || lineLength == 0)
  {
    delete[] words;
    return nullptr;
  }

  size_t pos = 0;
  while (pos < lineLength)
  {
    while (pos < lineLength && karpenko::isSpaceChar(line[pos]))
    {
      pos++;
    }

    if (pos >= lineLength)
    {
      break;
    }

    size_t wordStart = pos;

    while (pos < lineLength && !karpenko::isSpaceChar(line[pos]))
    {
      pos++;
    }
    size_t wordLength = pos - wordStart;

    if (wordLength > 0)
    {
      char* word = new char[wordLength + 1];
      std::memcpy(word, line + wordStart, wordLength);
      word[wordLength] = '\0';

      if (wordCount >= capacity)
      {
        size_t newCapacity = static_cast< size_t >(capacity * GROW_FACTOR);
        char** newWords = new char*[newCapacity];
        for (size_t i = 0; i < wordCount; ++i)
        {
          newWords[i] = words[i];
        }
        delete[] words;
        words = newWords;
        capacity = newCapacity;
      }
      words[wordCount] = word;
      wordCount++;
    }
  }

  delete[] line;

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

void karpenko::resizeCharArray(char*& array, size_t& capacity,
  size_t requiredSize)
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
