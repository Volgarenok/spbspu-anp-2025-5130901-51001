#include <iostream>
#include <iomanip>

namespace kitserov
{
  char* getline(std::istream& in, size_t& s);
  void removeLetters(const char* source, char* destination);
  void missLetters(const char* source, char* destination);
}

void kitserov::missLetters(const char* source, char* destination)
{
  size_t dstIndex = 0;
  const char* alphavit = "abcdefghijklmnopqrstuvwxyz";
  const size_t alphavitSize = 26;
  for (size_t j = 0; j < alphavitSize; ++j) {
    unsigned char alphavit_char = static_cast< unsigned char >(alphavit[j]);
    bool charInData = false;
    size_t i = 0;
    while (source[i] != '\0') {
      unsigned char source_char = static_cast< unsigned char >(std::tolower(source[i++]));
      if (alphavit_char == source_char) {
        charInData = true;
        break;
      }
    }
    if (!charInData) {
      destination[dstIndex++] = alphavit_char;
    }
  }
  destination[dstIndex] = '\0';
}

void kitserov::removeLetters(const char* source, char* destination)
{
  size_t srcIndex = 0;
  size_t dstIndex = 0;
  while (source[srcIndex] != '\0') {
    unsigned char source_char = static_cast< unsigned char >(source[srcIndex]);
    bool isLetter = std::isalpha(source_char) != 0;
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
  char* data = nullptr;
  data = kitserov::getline(std::cin, s);
  if (!data) {
    std::cerr << "Failed read line or memory allocation error\n";
    return 1;
  }
  if (s == 0) {
    std::cerr << "Empty input\n";
    free(data);
    return 1;
  }

  char* removedLetters = reinterpret_cast< char* >(malloc(s));
  if (!removedLetters) {
    std::cerr << "Failed memory allocation\n";
    free(data);
    return 1;
  }
  kitserov::removeLetters(data, removedLetters);

  char* missedLetters = reinterpret_cast< char* >(malloc(27));
  if (!missedLetters) {
    std::cerr << "Failed memory allocation\n";
    free(data);
    free(removedLetters);
    return 1;
  }
  kitserov::missLetters(data, missedLetters);

  std::cout << missedLetters << "\n";
  std::cout << removedLetters << "\n";
  free(data);
  free(removedLetters);
  free(missedLetters);
  return 0;
}
