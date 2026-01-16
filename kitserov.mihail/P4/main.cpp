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
  unsigned char uc = static_cast<unsigned char>(c);
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
    if (!in.fail() && letter != '\n') {
      in.putback(letter);
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

char** kitserov::readWords(std::istream& in, size_t& wordCount, int (*checkChar)(int))
{
  const size_t initialWordsCapacity = 8;
  char** words = reinterpret_cast< char** >(malloc(initialWordsCapacity * sizeof(char*)));
  if (!words) {
    wordCount = 0;
    return nullptr;
  }
  size_t wordsCapacity = initialWordsCapacity;
  wordCount = 0;
  std::ios::fmtflags originalFlags = in.flags();
  bool wasSkipws = (originalFlags & std::ios_base::skipws) != 0;
  if (!wasSkipws) {
      in >> std::skipws;
  }
  checkChar = isLetter;
  while (true) {
    size_t wordSize = 0;
    char* word = kitserov::getWord(in, wordSize);
    if (!word || wordSize == 0) {
      if (word) {
      	free(word);
      }
      break;
    }
    bool validWord = true;
    for (size_t i = 0; i < wordSize; ++i) {
      unsigned char c = static_cast< unsigned char >(word[i]);
      if (checkChar(c) == 0) {
        validWord = false;
        break;
      }
    }
    if (validWord) {
      if (wordCount >= wordsCapacity) {
        size_t newCapacity = wordsCapacity * 2;
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
    } else {
      free(word);
    }
    char nextChar;
    if (!(in >> nextChar)) {
      break;
    }
    if (nextChar == '\n') {
      in.putback(nextChar);
      break;
    } else if (nextChar == ' ' || nextChar == '\t') {
      continue;
    }
    in.putback(nextChar);
    break;
  }
  
  if (!wasSkipws) {
    in >> std::noskipws;
  }
  
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
  size_t s = 0;
  char* data = kitserov::getline(std::cin, s);
  if (!data) {
    std::cerr << "Failed read line or memory allocation error\n";
    return 1;
  }
  if (s == 0) {
    std::cerr << "Empty input\n";
    free(data);
    return 1;
  }

  char* removedLetters = reinterpret_cast< char* >(malloc(s + 1));
  if (!removedLetters) {
    std::cerr << "Failed memory allocation\n";
    free(data);
    return 1;
  }
  removedLetters[s] = '\0';
  kitserov::removeLetters(data, removedLetters);

  char* missedLetters = reinterpret_cast< char* >(malloc(kitserov::alphabetSize + 1));
  if (!missedLetters) {
    std::cerr << "Failed memory allocation\n";
    free(data);
    free(removedLetters);
    return 1;
  }
  missedLetters[kitserov::alphabetSize] = '\0';
  kitserov::missLetters(data, missedLetters);

  const char* str2 = "abs";
  int hasSame = kitserov::hasSameChars(data, str2);

  std::cout << missedLetters << "\n";
  std::cout << removedLetters << "\n";
  std::cout << hasSame << "\n";
  free(data);
  free(removedLetters);
  free(missedLetters);
  return 0;
}
