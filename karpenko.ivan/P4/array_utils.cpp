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

  char c;

  while (in.get(c) && isSpaceChar(c) && c != '\n')
  {}

  if (!in || c == '\n')
  {
    delete[] words;
    return nullptr;
  }

  in.putback(c);

  while (in.peek() != EOF && in.peek() != '\n')
  {
    size_t wordLength = 0;
    char* word = readStringWithAmortization(in, wordLength, isWordChar);

    if (wordLength == 0)
    {
      delete[] word;
      break;
    }

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

    while (in.get(c) && isSpaceChar(c) && c != '\n')
    {}

    if (!in || c == '\n')
    {
      break;
    }

    in.putback(c);
  }

  char** resizedWords = new char*[wordCount];
  for (size_t i = 0; i < wordCount; ++i)
  {
    resizedWords[i] = words[i];
  }
  delete[] words;

  return resizedWords;
}

char* karpenko::resizeCharArray(const char* array, size_t oldCapacity, size_t requiredSize, size_t& newCapacity)
{
  const double GROW_FACTOR = 1.5;
  newCapacity = static_cast< size_t >(oldCapacity * GROW_FACTOR);

  if (newCapacity <= oldCapacity)
  {
    newCapacity = oldCapacity + 1;
  }

  while (newCapacity < requiredSize)
  {
    newCapacity = static_cast< size_t >(newCapacity * GROW_FACTOR);
  }

  char* newArray = new char[newCapacity];

  if (array != nullptr)
  {
    std::memcpy(newArray, array, oldCapacity);
  }

  return newArray;
}
