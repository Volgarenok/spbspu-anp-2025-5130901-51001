#include <iostream>

namespace stepanov {
  char* getLine(std::istream& in, size_t& size);
  void toLower(char* str);
  void removeVowels(const char* str, char* output);
  bool isVowel(const char c);
}

int main()
{
  size_t size = 0;
  char* str = nullptr;
  try {
    str = stepanov::getLine(std::cin, size);
  } catch (const std::bad_alloc& e) {
    std::cerr << "Cannot read input\n";
    return 1;
  }
  if (str[0] == '\0') {
    delete[] str;
    std::cerr << "Input is empty\n";
    return 1;
  }

  stepanov::toLower(str);
  std::cout << str << '\n';

  char* withoutVowels = nullptr;
  try {
    withoutVowels = new char[size + 1];
  } catch (const std::bad_alloc& e) {
    delete[] str;
    std::cerr << "Cannot allocate memory\n";
    return 1;
  }
  withoutVowels[size] = '\0';
  stepanov::removeVowels(str, withoutVowels);
  std::cout << withoutVowels << '\n';
  delete[] withoutVowels;

  delete[] str;
  return 0;
}

char* stepanov::getLine(std::istream& in, size_t& size)
{
  const size_t step = 10;
  size_t capacity = 10;
  char* mem = new char[capacity];

  char c = in.get();
  while (!in.eof() && c != '\n') {
    if (size + 1 >= capacity) {
      char* new_mem = nullptr;
      try {
        new_mem = new char[capacity + step];
      } catch (const std::bad_alloc& e) {
        delete[] mem;
        throw;
      }
      capacity += step;
      for (size_t i = 0; i < size; i++) {
        new_mem[i] = mem[i];
      }
      delete[] mem;
      mem = new_mem;
    }
    mem[size] = c;
    size++;
    c = in.get();
  }
  mem[size] = '\0';
  return mem;
}

void stepanov::toLower(char* str)
{
  for (size_t i = 0; str[i] != '\0'; i++) {
    if (std::isalpha(str[i])) {
      str[i] = std::tolower(str[i]);
    }
  }
}

bool stepanov::isVowel(char c)
{
  switch (std::tolower(c)) {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
    case 'y':
      return true;
    default:
      return false;
  }
}

void stepanov::removeVowels(const char* str, char* output)
{
  size_t pos = 0;
  for (size_t i = 0; str[i] != '\0'; i++) {
    if (!stepanov::isVowel(str[i])) {
      output[pos] = str[i];
      pos++;
    }
  }
  output[pos] = '\0';
}
