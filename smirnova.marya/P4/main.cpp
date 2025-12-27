#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstring>

namespace smirnova
{
  char* getLine(std::istream &in, size_t& size);
  int compareStrings(const char *a, const char *b);
  void appendDigits(const char *a, const char *b, char *result);
}

char* smirnova::getLine(std::istream &in, size_t& size)
{
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws) {
    in >> std::noskipws;
  }

  size_t capacity = 16;
  size_t length = 0;

  char* str = reinterpret_cast< char* >(malloc(capacity));
  if (!str)
  {
    return 0;
  }

  char ch = '\0';
  while (in.get(ch) && ch != '\n')
  {
    if (length + 1 >= capacity)
    {
      size_t newCapacity = capacity * 2;
      char* buffer = reinterpret_cast< char* >(malloc(newCapacity));
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

  char* result = reinterpret_cast< char* >(malloc(length + 1));
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

  for (size_t i = 0; a[i] != '\0'; i++)
  {
    result[count++] = a[i];
  }

  for (size_t j = 0; b[j] != '\0'; j++)
  {
    if (std::isdigit(static_cast< unsigned char >(b[j]))) {
      result[count++] = b[j];
    }
  }

  result[count] = '\0';
}

int main()
{
  char* lines[2] = {nullptr, nullptr};
  size_t lengths[2] = {0, 0};

  char* line = smirnova::getLine(std::cin, lengths[0]);

  if (!line || (lengths[0] == 0 && !std::cin)) {
    std::cerr << "No input provided or memory allocation failed\n";
    return 1;
  }

  lines[0] = line;

  lines[1] = reinterpret_cast< char* >(malloc(8));
  if (!lines[1]) {
    std::cerr << "Memory allocation failed\n";
    free(lines[0]);
    return 1;
  }
  std::strcpy(lines[1], "default");
  lengths[1] = 7;

  int result1 = smirnova::compareStrings(lines[0], lines[1]);
  std::cout << result1 << "\n";

  size_t maxLen = lengths[0] + lengths[1] + 1;
  char* result2 = reinterpret_cast< char* >(malloc(maxLen+1));
  if (!result2) {
    std::cerr << "Memory allocation failed\n";
    free(lines[0]);
    free(lines[1]);
    return 1;
  }

  smirnova::appendDigits(lines[0], lines[1], result2);
  std::cout << result2 << "\n";

  free(result2);
  free(lines[0]);
  free(lines[1]);

  return 0;
}
