#include "getWords.hpp"
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <istream>

#include "getLine.hpp"

namespace smirnova
{
  char** getWords(std::istream& in, size_t& size,
    bool (*checkSpace)(char))
  {
    size = 0;
    size_t lineLen = 0;
    char* line = getLine(in, lineLen);
    if (!line) {
      return nullptr;
    }
    if (lineLen == 0) {
      free(line);
      return nullptr;
    }
    size_t cap = 10;
    char** words = reinterpret_cast< char** >(malloc(cap  * sizeof(char*)));
    if (!words) {
      free(line);
      return nullptr;
    }
    size_t countWords = 0, i = 0;
    while (i < lineLen) {
      while (i < lineLen && checkSpace(line[i])) {
        i++;
      }
      if (i >= lineLen) {
        break;
      }
      size_t start = i;
      while (i < lineLen && !checkSpace(line[i])) {
        i++;
      }
      size_t wordLen = i - start;
      char* word = reinterpret_cast< char* >(malloc(wordLen + 1));
      if (!word) {
        for (size_t j = 0; j < countWords; j++) {
          free(words[j]);
        }
        free(words);
        free(line);
        return nullptr;
      }
      for (size_t l = 0; l < wordLen; ++l) {
        word[l] = line[start + l];
      }
      word[wordLen] = '\0';
      if (countWords == cap) {
        size_t newCap = cap * 2;
        char** newWords = reinterpret_cast< char** >(malloc(newCap * sizeof(char*)));
        if (!newWords) {
          free(word);
          for (size_t k = 0; k < countWords; ++k) {
            free(words);
            free(line);
            return nullptr;
          }
        }
        std::memcpy(newWords, words, countWords * sizeof(char*));
        free(words);
        words = newWords;
        cap = newCap;
      }
      words[countWords++] = word;
    }
    size = countWords;
    free(line);
    return words;
  }
}
