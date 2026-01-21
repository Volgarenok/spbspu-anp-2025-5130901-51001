#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
<<<<<<< HEAD
=======
#include <istream>

#include "getLine.hpp"
#include "getWords.hpp"
>>>>>>> smirnova.marya/P4

namespace smirnova
{
  char *getLine(std::istream &in, size_t &size);
  char **getWords(std::istream &in, size_t &size, bool (*checkSpace)(char));
  int compareStrings(const char *a, const char *b);
  void appendDigits(const char *a, const char *b, char *result);
}

bool checkSpace(char ch) {
  return std::isspace(static_cast< unsigned char >(ch));
}

<<<<<<< HEAD
char **smirnova::getWords(std::istream &in, size_t &size,
  bool (*checkSpace)(char))
{
  size = 0;
  size_t lineLen = 0;
  char *line = getLine(in, lineLen);
  if (!line) {
    return nullptr;
  }
  if (lineLen == 0) {
    free(line);
    return nullptr;
  }
  size_t cap = 10;
  char **words = reinterpret_cast< char** >(malloc(cap  * sizeof(char*)));
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
    char *word = reinterpret_cast< char* >(malloc(wordLen + 1));
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
      char **newWords = reinterpret_cast< char** >(malloc(newCap * sizeof(char*)));
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

char *smirnova::getLine(std::istream &in, size_t &size)
{
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws) {
    in >> std::noskipws;
  }
  size_t capacity = 16;
  size_t length = 0;

  char *str = reinterpret_cast< char* >(malloc(capacity));
  if (!str) {
    return nullptr;
  }

  char ch = '\0';
  while (in.get(ch) && ch != '\n') {
    if (length + 1 >= capacity) {
      size_t newCapacity = capacity * 2;
      char *buffer = reinterpret_cast< char* >(malloc(newCapacity));
      if (!buffer) {
        free(str);
        return 0;
      }
      for (size_t i = 0; i < length; i++) {
        buffer[i] = str[i];
      }
      free(str);
      str = buffer;
      capacity = newCapacity;
    }
    str[length++] = ch;
  }

  if (is_skipws) {
    in >> std::skipws;
  }

  char *result = reinterpret_cast< char* >(malloc(length + 1));
  if (!result) {
    free(str);
    return 0;
  }

  for (size_t i = 0; i < length; i++) {
    result[i] = str[i];
  }
  result[length] = '\0';

  free(str);
  size = length;
  return result;
}
=======

>>>>>>> smirnova.marya/P4

int smirnova::compareStrings(const char *a, const char *b)
{
  int result = 0;
  for (size_t i = 0; a[i] != '\0'; i++) {
    for (size_t j = 0; b[j] != '\0'; j++) {
      if (a[i] == b[j]) {
        result++;
        break;
      }
    }
  }
  return result;
}

void smirnova::appendDigits(const char *a, const char *b, char *result)
{
  size_t count = 0;

  for (size_t i = 0; a[i] != '\0'; i++) {
    result[count++] = a[i];
  }

  for (size_t j = 0; b[j] != '\0'; j++) {
    if (std::isdigit(static_cast< unsigned char >(b[j]))) {
      for (size_t i = 0; a[i] != '\0'; i++) {
        if (a[i] == b[j]) {
          result[count++] = b[j];
          break;
        }
      }
    }
  }
  result[count] = '\0';
}

int main()
{
  std::cout << "\nEnter the line:\n";

  size_t countWords = 0;
  char **words = smirnova::getWords(std::cin, countWords, checkSpace);

  if (!words) {
    return 1;
  }
  if (countWords == 0) {
    free(words);
    return 1;
  }
  const char *defaultStr = "8default7";

  for (size_t i = 0; i < countWords; i++) {
    std::cout << "\n\nWords:\n" << words[i] << "\n" << defaultStr << "\n";

    int result1 = smirnova::compareStrings(words[i], defaultStr);
    std::cout << "\nResult1: " << result1 << "\n";

    size_t maxLen = strlen(words[i]) + strlen(defaultStr) + 1;
    char *result2 = reinterpret_cast< char* >(malloc(maxLen + 1));
    if (!result2) {
      for (size_t i = 0; i < countWords; i++) {
        free(words[i]);
      }
      free(words);
      return 1;
    } else {
      smirnova::appendDigits(words[i], defaultStr, result2);
      std::cout << "Result2: " << result2 << "\n";
      free(result2);
    }
  }
  std::cout << "\n";

  for (size_t i = 0; i < countWords; i++) {
    free(words[i]);
  }
  free(words);

  return 0;
}
