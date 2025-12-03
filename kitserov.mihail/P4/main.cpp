//14
//LAT_RMV #3
//malloc free
#include <iostream>
#include <iomanip>

namespace kitserov
{
  char* getline(std::istream& in, size_t& s);
  void removeLetters(const char* source, char* destination, size_t size);
  void missLetters(const char* source, char* destination, size_t size);
}

void kitserov::missLetters(const char* source, char* destination, size_t size)
{
  size_t dstIndex = 0;
  char alphavit[27] = "abcdefghijklmnopqrstuvwxyz";
  for (size_t j = 0; j < 27; ++j){
    unsigned char c = static_cast<unsigned char>(alphavit[j]);
    bool charInData = false;
    for (size_t i = 0; i < size; ++i) {
      unsigned char cc = static_cast<unsigned char>(std::tolower(source[i]));
      if (c == cc) {
        charInData = true;
        break;
      }
    }
    if (!charInData) {
      destination[dstIndex++] = c;
    }
  }
  destination[dstIndex] = '\0';
}

void kitserov::removeLetters(const char* source, char* destination, size_t size)
{
  size_t srcIndex = 0;
  size_t dstIndex = 0;
  while (source[srcIndex] != '\0' && dstIndex < size - 1) {
    unsigned char c = static_cast<unsigned char>(source[srcIndex]);
    bool isLetter = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    if (!isLetter) {
      destination[dstIndex] = source[srcIndex];
      dstIndex++;
    }
    srcIndex++;
  }
  destination[dstIndex] = '\0';
}

char* kitserov::getline(std::istream& in, size_t& s)
{
  bool isSkipws = in.flags() & std::ios_base::skipws;
  if (isSkipws) {
    in >> std::noskipws;
  }
  char* data = nullptr;
  data = static_cast<char*>(malloc(2));
  size_t size = 1;
  char letter;
  size_t i = 0;
  in >> letter;
  while (!(in.fail()) && letter != '\n') {
    if (i == size) {
      char* temp = static_cast<char*>(malloc(size * 2 + 1));
      for (size_t j = 0; j < size; j++) {
        temp[j] = data[j];
      }
      size *= 2;
      free(data);
      data = temp;
    }
    data[i] = letter;
    i++;
    in >> letter;
  }

  s = i;
  data[s] = '\0';
  if (isSkipws) {
    in >> std::skipws;
  }
  if (s == 0) {
    throw;
  }
  return data;
}

int main()
{
  size_t s = 0;
  char* data = nullptr;
  try {
    data = kitserov::getline(std::cin, s);
  } catch (...) {
    std::cerr << "Empty line";
    return 1;
  }
  if (!data) {
    std::cerr << "Failed read line\n";
    return 1;
  }

  char* removedLetters = static_cast<char*>(malloc(s + 1));
  if (!removedLetters) {
    std::cerr << "Failed memory allocation";
    free(data);
    return 1;
  }
  kitserov::removeLetters(data, removedLetters, s + 1);

  char* missedLetters = static_cast<char*>(malloc(28));
  if (!missedLetters) {
    std::cerr << "Failed memory allocation";
    free(data);
    free(removedLetters);
    return 1;
  }
  kitserov::missLetters(data, missedLetters, s + 1);

  std::cout << missedLetters << "\n";
  std::cout << removedLetters << "\n";
  free(data);
  free(removedLetters);
  free(missedLetters);
  return 0;
}
