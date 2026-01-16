#include "array_utils.hpp"
#include <iostream>
#include <cstring>
#include <cstddef>
#include <stdexcept>

namespace karpenko
{
  bool isWordChar(char ch)
  {
    return !std::isspace(static_cast< unsigned char >(ch));
  }

  char** readWords(std::istream& in, size_t& wordCount)
  {
    const size_t MAX_WORDS = 100;
    const size_t MAX_WORD_LEN = 256;
    
    char** words = nullptr;
    try
    {
      words = new char*[MAX_WORDS];
    }
    catch (const std::bad_alloc&)
    {
      wordCount = 0;
      throw;
    }
    
    char wordBuffer[MAX_WORD_LEN];
    wordCount = 0;
    char c;
    bool inWord = false;
    size_t wordLen = 0;
    
    auto finishWord = [&]()
      {
        if (inWord && wordLen > 0)
        {
          if (wordCount >= MAX_WORDS)
          {
            for (size_t i = 0; i < wordCount; ++i)
            {
              delete[] words[i];
            }
            delete[] words;
            throw std::runtime_error("too many words");
          }
          
          char* wordCopy = nullptr;
          try
          {
            wordCopy = new char[wordLen + 1];
          }
          catch (const std::bad_alloc&)
          {
            for (size_t i = 0; i < wordCount; ++i)
            {
              delete[] words[i];
            }
            delete[] words;
            throw;
          }
          
          std::memcpy(wordCopy, wordBuffer, wordLen);
          wordCopy[wordLen] = '\0';
          words[wordCount] = wordCopy;
          wordCount++;
          inWord = false;
          wordLen = 0;
        }
      };
    
    try
    {
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
            wordLen = 0;
          }
          
          if (wordLen >= MAX_WORD_LEN - 1)
          {
            for (size_t i = 0; i < wordCount; ++i)
            {
              delete[] words[i];
            }
            delete[] words;
            throw std::runtime_error("word too long");
          }
          
          wordBuffer[wordLen++] = c;
        }
        else
        {
          finishWord();
        }
      }
      
      finishWord();
    }
    catch (...)
    {
      for (size_t i = 0; i < wordCount; ++i)
      {
        delete[] words[i];
      }
      delete[] words;
      wordCount = 0;
      throw;
    }
    
    if (wordCount == 0)
    {
      delete[] words;
      return nullptr;
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
      throw;
    }
    
    for (size_t i = 0; i < wordCount; ++i)
    {
      resizedWords[i] = words[i];
    }
    delete[] words;
    
    return resizedWords;
  }
}
