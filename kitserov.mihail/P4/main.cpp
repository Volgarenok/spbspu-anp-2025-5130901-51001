#include <iostream>
#include <iomanip>

namespace kitserov
{
  char* getline(std::istream& in, size_t& size);
  void removeLetters(const char* source, char* destination);
  void missLetters(const char* source, char* destination);
  const size_t alphabetSize = 26;
  int hasSameChars(const char* str1, const char* str2);
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
  const size_t alphavitSize = alphabetSize;
  for (size_t j = 0; j < alphavitSize; ++j) {
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

char* kitserov::getline(std::istream& in, size_t& size)
{
  bool isSkipws = in.flags() & std::ios_base::skipws;
  if (isSkipws) {
    in >> std::noskipws;
  }
  char* data = reinterpret_cast< char* >(malloc(2));
  if (!data) {
    return nullptr;
  }
  size_t capacity = 1;
  char letter;
  size = 0;
  in >> letter;
  while (!(in.fail()) && letter != '\n') {
    if (size == capacity) {
      char* temp = reinterpret_cast< char* >(malloc(capacity * 2 + 1));
      if (!temp) {
        free(data);
        if (isSkipws) {
          in >> std::skipws;
        }
        return nullptr;
      }
      for (size_t j = 0; j < capacity; j++) {
        temp[j] = data[j];
      }
      capacity *= 2;
      free(data);
      data = temp;
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
