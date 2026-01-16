#include <iostream>
#include <iomanip>

namespace kitserov
{
  char* getline(std::istream& in, size_t& size, size_t capacity);
  void removeLetters(const char* source, char* destination);
  void missLetters(const char* source, char* destination);
  int hasSameChars(const char* str1, const char* str2);

  char* getlineAmortized(std::istream& in, size_t& size, size_t& capacity);
  char* getWord(std::istream& in, size_t& size);
  int isLetter(int c);
  char** readWords(std::istream& in, size_t& wordCount, int (*checkChar)(int) = nullptr);
  void freeWords(char** words, size_t count);

  const size_t alphabetSize = 26;
  const size_t initialCapacity = 8;
  const double expansion = 2;
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

int kitserov::isLetter(int c)
{
  unsigned char uc = static_cast< unsigned char >(c);
  return (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z');
}

char* kitserov::getline(std::istream& in, size_t& size, size_t capacity)
{
  bool isSkipws = in.flags() & std::ios_base::skipws;
  if (isSkipws) {
    in >> std::noskipws;
  }
  char* data = reinterpret_cast< char* >(malloc(capacity));
  if (!data) {
    return nullptr;
  }
  char letter;
  size = 0;
  in >> letter;
  while (!(in.fail()) && letter != '\n') {
    if (size + 1 >= capacity) {
      size_t newCapacity = static_cast< size_t >(capacity * expansion + 1);
      char* temp = reinterpret_cast< char* >(malloc(newCapacity));
      if (!temp) {
        free(data);
        size = 0;
        if (isSkipws) {
          in >> std::skipws;
        }
        return nullptr;
      }
      for (size_t j = 0; j < capacity; j++) {
        temp[j] = data[j];
      }
      free(data);
      data = temp;
      capacity = newCapacity;
    }
    data[size] = letter;
    size++;
    in >> letter;
  }

  data[size] = '\0';
  if (isSkipws) {
    in >> std::skipws;
  }
  return data;
}

char* kitserov::getWord(std::istream& in, size_t& size)
{
  bool isSkipws = in.flags() & std::ios_base::skipws;
  if (!isSkipws) {
    in >> std::skipws;
  }
  char* data = reinterpret_cast< char* >(malloc(2));
  if (!data) {
    if (!isSkipws) {
      in >> std::noskipws;
    }
    return nullptr;
  }
  size_t capacity = initialCapacity;
  char letter;
  size = 0;
  in >> letter;
  while (!(in.fail()) && letter != '\n' && letter != ' ') {
    if (size + 1 >= capacity) {
      size_t newCapacity = static_cast< size_t >(capacity * expansion + 1);
      char* temp = reinterpret_cast< char* >(malloc(newCapacity));
      if (!temp) {
        free(data);
        size = 0;
        if (isSkipws) {
          in >> std::skipws;
        }
        return nullptr;
      }
      for (size_t j = 0; j < capacity; j++) {
        temp[j] = data[j];
      }
      free(data);
      data = temp;
      capacity = newCapacity;
    }
    data[size] = letter;
    size++;
    if (!(in >> letter)) {
      break;
    }
  }
  if (!in.fail() && letter != '\n') {
    in.putback(letter);
  }
  data[size] = '\0';
  if (isSkipws) {
    in >> std::skipws;
  }
  return data;

}

char** kitserov::readWords(std::istream& in, size_t& wordCount, int (*checkChar)(int))
{
  size_t lineSize = 0;
  char* line = kitserov::getline(in, lineSize, initialCapacity);
  if (!line || lineSize == 0) {
    if (line) {
      free(line);
    }
    wordCount = 0;
    return nullptr;
  }
  const size_t initialWordsCapacity = 8;
  char** words = reinterpret_cast< char** >(malloc(initialWordsCapacity * sizeof(char*)));
  if (!words) {
    free(line);
    wordCount = 0;
    return nullptr;
  }
  size_t wordsCapacity = initialWordsCapacity;
  wordCount = 0;
  int (*checkFunc)(int) = checkChar;
  if (!checkFunc) {
    checkFunc = [](int) -> int { return 1; };
  }
  size_t start = 0;
  size_t i = 0;
  while (i <= lineSize) {
    if (i == lineSize || line[i] == ' ') {
      if (start < i) {
        size_t wordLen = i - start;
        bool validWord = true;
        for (size_t j = start; j < i; ++j) {
          unsigned char c = static_cast< unsigned char >(line[j]);
          if (checkFunc(c) == 0) {
            validWord = false;
            break;
          }
        }
        if (validWord) {
          char* word = reinterpret_cast< char* >(malloc(wordLen + 1));
          if (word) {
            for (size_t j = 0; j < wordLen; ++j) {
              word[j] = line[start + j];
            }
            word[wordLen] = '\0';
            if (wordCount >= wordsCapacity) {
              size_t newCapacity = wordsCapacity * expansion;
              char** temp = reinterpret_cast< char** >(realloc(words, newCapacity * sizeof(char*)));
              if (!temp) {
                free(word);
                break;
              }
              words = temp;
              wordsCapacity = newCapacity;
            }
            words[wordCount] = word;
            wordCount++;
          }
        }
      }
      start = i + 1;
    }
    i++;
  }
  free(line);
  if (wordCount == 0) {
    free(words);
    return nullptr;
  }
  return words;
}

void kitserov::freeWords(char** words, size_t count)
{
  if (!words) {
  	return;
  }
  for (size_t i = 0; i < count; ++i) {
    free(words[i]);
  }
  free(words);
}


int main()
{
  size_t wordCount = 0;
  std::cout << "Enter words: ";
  char** words = kitserov::readWords(std::cin, wordCount, nullptr);
  if (!words) {
    std::cerr << "Failed to read words or no valid words found\n";
    return 1;
  }
  if (wordCount == 0) {
    std::cerr << "No words found\n";
    free(words);
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
