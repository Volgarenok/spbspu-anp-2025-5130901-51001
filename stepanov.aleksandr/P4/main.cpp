#include <iostream>

namespace stepanov {
  char* getline(std::istream& in);
  void to_lower(char* str);
  void remove_vowels(char* str);
  bool is_vowel(const char c);
}

int main() {
  char* str = stepanov::getline(std::cin);
  if (str == nullptr) {
    std::cerr << "Cannot read input\n";
    return 1;
  }
  if (str[0] == '\0') {
    delete[] str;
    std::cerr << "Input is empty\n";
    return 1;
  }
  stepanov::to_lower(str);
  std::cout << str << std::endl;
  stepanov::remove_vowels(str);
  std::cout << str << std::endl;
  delete[] str;
  return 0;
}

char* stepanov::getline(std::istream& in) {
  const size_t step = 10;
  size_t len = 0;
  size_t mem_len = 10;
  char* mem = nullptr;
  try {
    mem = new char[mem_len];
  } catch (std::bad_alloc& e) {
    return nullptr;
  }

  while (1) {
    char c = in.get();
    if (c == EOF || c == '\n') {
      break;
    }
    if (len + 1 >= mem_len) {
      mem_len += step;
      char* new_mem = nullptr;
      try {
        new_mem = new char[mem_len];
      } catch (std::bad_alloc& e) {
        delete[] mem;
        return nullptr;
      }
      for (size_t i = 0; i < len; i++) {
        new_mem[i] = mem[i];
      }
      delete[] mem;
      mem = new_mem;
    }
    mem[len] = c;
    len++;
  }
  mem[len] = '\0';
  return mem;
}

void stepanov::to_lower(char* str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] += 32;
    }
  }
}

bool stepanov::is_vowel(char c) {
  c = tolower(c);
  switch (c) {
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

void stepanov::remove_vowels(char* str) {
  int pos = 0;
  for (size_t i = 0; str[i] != '\0'; i++) {
    if (stepanov::is_vowel(str[i])) {
      continue;
    }
    str[pos] = str[i];
    pos++;
  }
  str[pos] = '\0';
}
