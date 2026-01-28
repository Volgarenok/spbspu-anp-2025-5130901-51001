#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <istream>

#include "getLine.hpp"
#include "getWords.hpp"

namespace smirnova
{
  char* getLine(std::istream& in, size_t& size);
  char** getWords(std::istream& in, size_t& size, bool (*checkSpace)(char));
  int compareStrings(const char* a, const char* b);
  void appendDigits(const char* a, const char* b, char* result);
}

bool checkSpace(char ch) {
  return std::isspace(static_cast< unsigned char >(ch));
}



int smirnova::compareStrings(const char* a, const char* b)
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

void smirnova::appendDigits(const char* a, const char* b, char* result)
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
  char** words = smirnova::getWords(std::cin, countWords, checkSpace);

  if (!words) {
    std::cerr << "Error: Failed to read words from input.\n";
    return 1;
  }
  if (countWords == 0) {
    std::cerr << "Warning: No words entered.\n";
    free(words);
    return 1;
  }
  const char* defaultStr = "8default7";

  for (size_t i = 0; i < countWords; i++) {
    std::cout << "\n\nWords:\n" << words[i] << "\n" << defaultStr << "\n";

    int result1 = smirnova::compareStrings(words[i], defaultStr);
    std::cout << "\nResult1: " << result1 << "\n";

    size_t maxLen = strlen(words[i]) + strlen(defaultStr) + 1;
    char* result2 = reinterpret_cast< char* >(malloc(maxLen + 1));
    if (!result2) {
      std::cerr << "Error: Failed to allocate memory for result.\n";
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
