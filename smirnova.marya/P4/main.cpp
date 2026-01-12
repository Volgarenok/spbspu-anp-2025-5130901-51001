#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace smirnova {
char *getLine(std::istream &in, size_t &size);
char **getWords(std::istream &in, size_t &size, bool (*checkSpace)(char));
int compareStrings(const char *a, const char *b);
void appendDigits(const char *a, const char *b, char *result);
} // namespace smirnova

bool checkSpace(char ch) {
  return std::isspace(static_cast<unsigned char>(ch));
}

char **smirnova::getWords(std::istream &in, size_t &size,
                          bool (*checkSpace)(char)) {
  const size_t MAX_WORDS = 100;
  const size_t INITIAL_BUF = 16;
  char *buffer = (char *)malloc(INITIAL_BUF);
  if (!buffer) {
    return nullptr;
  }
  size_t bufCap = INITIAL_BUF;
  size_t bufLen = 0;
  char **words = static_cast<char **>(malloc(MAX_WORDS * sizeof(char *)));
  if (!words) {
    free(buffer);
    return nullptr;
  }
  size_t countWords = 0;
  char ch;
  bool inWord = false;
  while (in.get(ch) && ch != '\n') {
    if (checkSpace(ch)) {
      if (inWord && bufLen > 0) {
        char *word = static_cast<char *>(malloc(bufLen + 1));
        if (!word) {
          free(buffer);
          return nullptr;
        }
        for (size_t i = 0; i < bufLen; i++) {
          word[i] = buffer[i];
        }
        word[bufLen] = '\0';

        if (countWords < MAX_WORDS) {
          words[countWords++] = word;
        } else {
          free(word);
        }
        bufLen = 0;
        inWord = false;
      }
    } else {
      if (bufLen + 1 >= bufCap) {
        size_t newCap = bufCap * 2;
        char *newBuffer = static_cast<char *>(malloc(newCap));
        if (!newBuffer) {
          break;
        }
        for (size_t i = 0; i < bufLen; i++) {
          newBuffer[i] = buffer[i];
        }
        free(buffer);
        buffer = newBuffer;
        bufCap = newCap;
      }
      buffer[bufLen++] = ch;
      inWord = true;
    }
  }
  if (inWord && bufLen > 0 && (countWords < MAX_WORDS)) {
    char *word = static_cast<char *>(malloc(bufLen + 1));
    if (!word) {
      free(buffer);
      return nullptr;
    } else {
      for (size_t i = 0; i < bufLen; i++) {
        word[i] = buffer[i];
      }
      word[bufLen] = '\0';
      words[countWords++] = word;
    }
  }
  free(buffer);
  size = countWords;
  return words;
}

char *smirnova::getLine(std::istream &in, size_t &size) {
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws) {
    in >> std::noskipws;
  }

  size_t capacity = 16;
  size_t length = 0;

  char *str = reinterpret_cast<char *>(malloc(capacity));
  if (!str) {
    return 0;
  }

  char ch = '\0';
  while (in.get(ch) && ch != '\n') {
    if (length + 1 >= capacity) {
      size_t newCapacity = capacity * 2;
      char *buffer = reinterpret_cast<char *>(malloc(newCapacity));
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

  char *result = reinterpret_cast<char *>(malloc(length + 1));
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

int smirnova::compareStrings(const char *a, const char *b) {
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

void smirnova::appendDigits(const char *a, const char *b, char *result) {
  size_t count = 0;

  for (size_t i = 0; a[i] != '\0'; i++) {
    result[count++] = a[i];
  }

  for (size_t j = 0; b[j] != '\0'; j++) {
    if (std::isdigit(static_cast<unsigned char>(b[j]))) {
      result[count++] = b[j];
    }
  }

  result[count] = '\0';
}

int main() {
  std::cout << "\n-----------------------------------------------------\n";
  std::cout << "Enter the line for the main task:\n";
  char *lines[2] = {nullptr, nullptr};
  size_t lengths[2] = {0, 0};

  char *line = smirnova::getLine(std::cin, lengths[0]);
  std::cout << "-----------------------------------------------------";

  if (!line || (lengths[0] == 0 && !std::cin)) {
    std::cerr << "\nNo input provided or memory allocation failed\n";
    free(line);
    return 1;
  }

  lines[0] = line;

  lines[1] = reinterpret_cast<char *>(malloc(8));
  if (!lines[1]) {
    std::cerr << "\nMemory allocation failed\n";
    free(lines[0]);
    return 0;
  }
  std::strcpy(lines[1], "default12");
  lengths[1] = 7;

  int result1 = smirnova::compareStrings(lines[0], lines[1]);
  std::cout << "\nStrings:\n" << lines[0] << "\n" << lines[1] << "\n";
  std::cout << "-----------------------------------------------------";
  std::cout << "\nResult 1: " << result1 << "\n";

  size_t maxLen = lengths[0] + lengths[1];
  char *result2 = reinterpret_cast<char *>(malloc(maxLen + 1));
  if (!result2) {
    std::cerr << "\nMemory allocation failed\n";
    free(lines[0]);
    free(lines[1]);
    return 1;
  }

  smirnova::appendDigits(lines[0], lines[1], result2);
  std::cout << "Result 2: " << result2 << "\n";

  free(result2);
  free(lines[0]);
  free(lines[1]);

  std::cout << "\n-----------------------------------------------------\n";
  std::cout << "Enter the line for the additional task:" << "\n";

  size_t countWords = 0;
  char **words = smirnova::getWords(std::cin, countWords, checkSpace);
  std::cout << "-----------------------------------------------------";

  if (!words || countWords == 0) {
    std::cerr << "\nMemory allocation failed or nothing entered\n";
    if (words) {
      free(words);
      return 1;
    }
  }
  const char *defaultStr = "8default7";

  for (size_t i = 0; i < countWords; i++) {
    std::cout << "\nWords:\n" << words[i] << "\n" << defaultStr << "\n";

    int result3 = smirnova::compareStrings(words[i], defaultStr);
    std::cout << "-----------------------------------------------------";
    std::cout << "\nResult3: " << result3 << "\n";

    size_t maxLen = strlen(words[i]) + strlen(defaultStr) + 1;
    char *result4 = static_cast<char *>(malloc(maxLen));
    if (!result4) {
      return 0;
    } else {
      smirnova::appendDigits(words[i], defaultStr, result4);
      std::cout << "Result4: " << result4 << "\n";
      free(result4);
    }
  }
  std::cout << "\n";

  for (size_t i = 0; i < countWords; i++) {
    free(words[i]);
  }
  free(words);

  return 0;
}
