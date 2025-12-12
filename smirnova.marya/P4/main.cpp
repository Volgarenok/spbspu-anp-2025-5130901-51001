#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstring>

namespace smirnova
{
  size_t getLine(std::istream &in, char *&result);
  char* getStr(std::istream &in, bool &flag, size_t &length);
  void compareStrings(const char *a, const char *b, int *result);
  void appendDigits(const char *a, const char *b, char *result, size_t maxLen);
}

size_t smirnova::getLine(std::istream &in, char *&result)
{
  result = nullptr;
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws) {
    in >> std::noskipws;
  }

  size_t capacity = 16;
  size_t length = 0;

  char* str = reinterpret_cast<char*>(malloc(capacity));
  if (!str)
  {
    return 0;
  }

  char ch = '\0';
  while (in.get(ch))
  {
    if (ch == '\n') {
      break;
    }

    if (length + 1 >= capacity)
    {
      size_t newCapacity = capacity * 2;
      char* buffer = reinterpret_cast<char*>(malloc(newCapacity));
      if (!buffer)
      {
        free(str);
        return 0;
      }
      for (size_t i = 0; i < length; i++)
      {
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

  if (length == 0 && in.eof()) {
    free(str);
    return 0;
  }

  result = static_cast<char*>(malloc(length + 1));
  if (!result) {
    free(str);
    return 0;
  }

  for (size_t i = 0; i < length; i++) {
    result[i] = str[i];
  }
  result[length] = '\0';

  free(str);
  return length;
}

char* smirnova::getStr(std::istream &in, bool &flag, size_t &length)
{
  flag = false;
  char* line = nullptr;
  length = getLine(in, line);

  while (line != nullptr && length == 0) {
    free(line);
    length = getLine(in, line);

    if (in.eof()) {
      if (line != nullptr) {
        free(line);
      }
      line = nullptr;
      break;
    }
  }

  if (line != nullptr && length > 0) {
    flag = true;
    return line;
  }

  if (line != nullptr) {
    free(line);
  }
  flag = false;
  return nullptr;
}

void smirnova::compareStrings(const char *a, const char *b, int *result)
{
  *result = 0;
  for (size_t i = 0; a[i] != '\0'; i++) {
    for (size_t j = 0; b[j] != '\0'; j++) {
      if (a[i] == b[j]) {
        *result = 1;
        return;
      }
    }
  }
}

void smirnova::appendDigits(const char *a, const char *b, char *result, size_t maxLen)
{
  size_t count = 0;

  for (size_t i = 0; a[i] != '\0' && count < maxLen - 1; i++)
  {
    result[count++] = a[i];
  }

  for (size_t j = 0; b[j] != '\0' && count < maxLen - 1; j++)
  {
    if (std::isdigit(static_cast<unsigned char>(b[j]))) {
      result[count++] = b[j];
    }
  }

  result[count] = '\0';
}

int main()
{
  char* lines[2] = {nullptr, nullptr};
  size_t lengths[2] = {0, 0};

  try {
    int countLines = 0;
    bool flag = false;

    while (countLines < 2)
    {
      char* line = smirnova::getStr(std::cin, flag, lengths[countLines]);

      if (!flag)
      {
        std::cerr << "Memory allocation failed\n";
        if (lines[0]) {
          free(lines[0]);
        }
        return 1;
      }

      if (!line) {
        if (countLines == 0) {
          std::cerr << "No input provided.\n";
          return 1;
        }
        break;
      }

      lines[countLines++] = line;
    }

    if (countLines == 1) {
      lines[1] = static_cast<char*>(malloc(8));
      if (!lines[1]) {
        std::cerr << "Memory allocation failed\n";
        free(lines[0]);
        return 1;
      }
      std::strcpy(lines[1], "default");
      lengths[1] = 7;
      countLines++;
    }

    int result1 = 0;
    smirnova::compareStrings(lines[0], lines[1], &result1);
    std::cout << result1 << "\n";

    size_t maxLen = lengths[0] + lengths[1] + 1;
    char* result2 = reinterpret_cast<char*>(malloc(maxLen));
    if (!result2) {
      std::cerr << "Memory allocation failed\n";
      free(lines[0]);
      free(lines[1]);
      return 1;
    }

    smirnova::appendDigits(lines[0], lines[1], result2, maxLen);
    std::cout << result2 << "\n";

    free(result2);
    free(lines[0]);
    free(lines[1]);

    return 0;
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    if (lines[0]) {
      free(lines[0]);
    }
    if (lines[1]) {
      free(lines[1]);
    }
    return 1;
  }
}
