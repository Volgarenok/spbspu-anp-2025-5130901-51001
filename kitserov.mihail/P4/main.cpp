#include <iostream>
#include <iomanip>

namespace kitserov
{
  char** getline(std::istream& in, size_t& wordCount, int (*checkFunc)(int));
  void removeLetters(const char* source, char* destination);
  void missLetters(const char* source, char* destination);
  int hasSameChars(const char* str1, const char* str2);
  void freeWords(char** words, size_t count);
  int checkSpace(int c);

  constexpr size_t alphabetSize = 26;
  constexpr size_t initialCapacity = 8;
  constexpr double expansion = 2;
}

int kitserov::hasSameChars(const char* str1, const char* str2)
{
  unsigned char charSet[256] = {0};
  for (size_t i = 0; str1[i] != '\0'; ++i) {
    unsigned char c = static_cast< unsigned char >(str1[i]);
    charSet[c] = 1;
  }
  for (size_t i = 0; str2[i] != '\0'; ++i) {
    unsigned char c = static_cast< unsigned char >(str2[i]);
    if (charSet[c] != 0) {
      return 1;
    }
  }
  return 0;
}

void kitserov::missLetters(const char* source, char* destination)
{
  size_t dstIndex = 0;
  const char* alphavit = "abcdefghijklmnopqrstuvwxyz";
  for (size_t j = 0; alphavit[j] != '\0'; ++j) {
    unsigned char alphavitChar = static_cast< unsigned char >(alphavit[j]);
    bool charInData = false;
    size_t i = 0;
    while (source[i] != '\0') {
      unsigned char sourceChar = static_cast< unsigned char >(std::tolower(source[i++]));
      if (alphavitChar == sourceChar) {
        charInData = true;
        break;
      }
    }
    if (!charInData) {
      destination[dstIndex++] = alphavitChar;
    }
  }
  destination[dstIndex] = '\0';
}

void kitserov::removeLetters(const char* source, char* destination)
{
  size_t srcIndex = 0;
  size_t dstIndex = 0;
  while (source[srcIndex] != '\0') {
    unsigned char sourceChar = static_cast< unsigned char >(source[srcIndex]);
    bool isLetter = std::isalpha(sourceChar) != 0;
    if (!isLetter) {
      destination[dstIndex] = source[srcIndex];
      dstIndex++;
    }
    srcIndex++;
  }
  destination[dstIndex] = '\0';
}

char** kitserov::getline(std::istream& in, size_t& wordCount, int (*checkFunc)(int))
{
  bool isSkipws = in.flags() & std::ios_base::skipws;
  if (isSkipws) {
    in >> std::noskipws;
  }
  wordCount = 0;
  size_t wordCapacity = initialCapacity;
  size_t wordsCapacity = initialCapacity;
  size_t wordSize = 0;
  char** words = reinterpret_cast< char** >(malloc(wordsCapacity * sizeof(char*)));
  if (!words) {
    if (isSkipws) {
      in >> std::skipws;
    }
    return nullptr;
  }
  char* currentWord = reinterpret_cast< char* >(malloc(wordCapacity));
  if (!currentWord) {
    free(words);
    if (isSkipws) {
      in >> std::skipws;
    }
    return nullptr;
  }
  char letter;
  in >> letter;
  if (in.fail()) {
    free(currentWord);
    free(words);
    if (isSkipws) {
      in >> std::skipws;
    }
    return nullptr;
  }
  while (!(in.fail())) {
    if (checkFunc(static_cast< unsigned char >(letter))) {
      if (wordSize > 0) {
        currentWord[wordSize] = '\0';
        if (wordCount >= wordsCapacity) {
          size_t newCapacity = static_cast< size_t >(wordsCapacity * expansion);
          char** temp = reinterpret_cast< char** >(malloc(newCapacity * sizeof(char*)));
          if (!temp) {
            free(currentWord);
            freeWords(words, wordCount);
            if (isSkipws) {
              in >> std::skipws;
            }
            return nullptr;
          }
          for (size_t i = 0; i < wordCount; i++) {
            temp[i] = words[i];
          }
          free(words);
          words = temp;
          wordsCapacity = newCapacity;
        }
        words[wordCount] = currentWord;
        wordCount++;

        wordSize = 0;
        wordCapacity = initialCapacity;
        currentWord = reinterpret_cast< char* >(malloc(wordCapacity));
        if (!currentWord) {
          freeWords(words, wordCount);
          if (isSkipws) {
            in >> std::skipws;
          }
          return nullptr;
        }
      }
    } else {
      if (wordSize + 1 >= wordCapacity) {
        size_t newCapacity = static_cast< size_t >(wordCapacity * expansion);
        char* temp = reinterpret_cast< char* >(malloc(newCapacity));
        if (!temp) {
          freeWords(words, wordCount);
          free(currentWord);
          if (isSkipws) {
            in >> std::skipws;
          }
          return nullptr;
        }
        for (size_t i = 0; i < wordSize; i++) {
          temp[i] = currentWord[i];
        }
        free(currentWord);
        currentWord = temp;
        wordCapacity = newCapacity;
      }
      currentWord[wordSize] = letter;
      wordSize++;
    }
    in >> letter;
    if (in.fail() || letter == '\n') {
      if (wordSize > 0) {
        currentWord[wordSize] = '\0';
        if (wordCount >= wordsCapacity) {
          size_t newCapacity = static_cast< size_t >(wordsCapacity * expansion);
          char** temp = reinterpret_cast< char** >(malloc(newCapacity * sizeof(char*)));
          if (!temp) {
            free(currentWord);
            freeWords(words, wordCount);
            if (isSkipws) {
              in >> std::skipws;
            }
            return nullptr;
          }
          for (size_t i = 0; i < wordCount; i++) {
            temp[i] = words[i];
          }
          free(words);
          words = temp;
          wordsCapacity = newCapacity;
        }
        words[wordCount] = currentWord;
        wordCount++;
      } else {
        free(currentWord);
      }
      break;
    }
  }
  if (wordCount == 0) {
    free(words);
    words = nullptr;
  }
  if (isSkipws) {
    in >> std::skipws;
  }
  return words;
}

void kitserov::freeWords(char** words, size_t count)
{
  if (!words) {
    return;
  }
  for (size_t i = 0; i < count; ++i) {
    if (words[i]) {
      free(words[i]);
    }
  }
  free(words);
}

int kitserov::checkSpace(int c)
{
  return c == ' ';
}

int main()
{
  size_t wordCount = 0;
  std::cout << "Enter words: ";
  char** words = kitserov::getline(std::cin, wordCount, kitserov::checkSpace);
  if (!words) {
    std::cerr << "Failed to read words or no valid words found\n";
    return 1;
  }
  const char* str2 = "abs";
  for (size_t i = 0; i < wordCount; ++i) {
    const char* currentWord = words[i];
    size_t wordLen = 0;
    while (currentWord[wordLen] != '\0') {
      wordLen++;
    }
    std::cout << i;
    char* missedLetters = reinterpret_cast< char* >(malloc(kitserov::alphabetSize + 1));
    if (missedLetters) {
      missedLetters[kitserov::alphabetSize] = '\0';
      kitserov::missLetters(currentWord, missedLetters);
      std::cout << " " << missedLetters;
      free(missedLetters);
    } else {
      std::cerr << "Failed memory allocation\n";
      kitserov::freeWords(words, wordCount);
      return 1;
    }
    char* removedLetters = reinterpret_cast< char* >(malloc(wordLen + 1));
    if (removedLetters) {
      removedLetters[wordLen] = '\0';
      kitserov::removeLetters(currentWord, removedLetters);
      std::cout << " " << removedLetters;
      free(removedLetters);
    } else {
      std::cerr << "Failed memory allocation\n";
      kitserov::freeWords(words, wordCount);
      return 1;
    }
    int hasSame = kitserov::hasSameChars(currentWord, str2);
    std::cout << " " << hasSame << "\n";
  }
  kitserov::freeWords(words, wordCount);
  return 0;
}
