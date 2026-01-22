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

bool karpenko::skipSpaces(std::istream& in, char& c)
{
  while (in.get(c) && isSpaceChar(c) && c != '\n')
  {}

  return !(!in || c == '\n');
}

char** karpenko::readWords(std::istream& in, size_t& wordCount)
{
  const size_t INITIAL_CAPACITY = 16;
  const double GROW_FACTOR = 1.5;

  size_t capacity = INITIAL_CAPACITY;
  char** words = nullptr;
  wordCount = 0;

  try
  {
    words = new char*[capacity];
  }
  catch (const std::bad_alloc&)
  {
    return nullptr;
  }

  char c;

  if (!skipSpaces(in, c))
  {
    delete[] words;
    return nullptr;
  }

  in.putback(c);

  while (in.peek() != EOF && in.peek() != '\n')
  {
    size_t wordLength = 0;
    char* word = nullptr;

    try
    {
      word = readStringWithAmortization(in, wordLength, isWordChar);
    }
    catch (const std::bad_alloc&)
    {
      for (size_t i = 0; i < wordCount; ++i)
      {
        delete[] words[i];
      }
      delete[] words;
      return nullptr;
    }

    if (wordLength == 0)
    {
      delete[] word;
      break;
    }

    if (wordCount >= capacity)
    {
      size_t newCapacity = static_cast< size_t >(capacity * GROW_FACTOR);
      char** newWords = nullptr;

      try
      {
        newWords = new char*[newCapacity];
      }
      catch (const std::bad_alloc&)
      {
        delete[] word;
        for (size_t i = 0; i < wordCount; ++i)
        {
          delete[] words[i];
        }
        delete[] words;
        return nullptr;
      }

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

    if (!skipSpaces(in, c))
    {
      break;
    }

    in.putback(c);
  }

  char** resizedWords = nullptr;

  try
  {
    resizedWords = new char*[wordCount];
  }
  catch (const std::bad_alloc&)
  {
    for (size_t i = 0; i < wordCount; ++i)
    {
      delete[] words[i];
    }
    delete[] words;
    return nullptr;
  }

  for (size_t i = 0; i < wordCount; ++i)
  {
    resizedWords[i] = words[i];
  }
  delete[] words;

  return resizedWords;
}
