#include <iostream>
#include <istream>
#include <cctype>

namespace loseva
{
  char* lat_two(const char* str1, const char* str2, char* result, size_t result>
  char* lat_rmv(const char* src, char* result, size_t result_size);
  char* mergeStrings(char* a, char* b, size_t as, size_t bs);
  char* readInputLine(std::istream& in, size_t& size);
}

char* loseva::mergeStrings(char* a, char* b, size_t as, size_t bs)
{
  size_t len = as + bs;
  char* result = new char[len];

  for (size_t i = 0; i < as; i++) {
    result[i] = a[i];
  }

  for (size_t i = 0; i < bs; i++) {
    result[as + i] = b[i];
  }

  return result;
}

char* loseva::readInputLine(std::istream& in, size_t& size)
{
  size_t batchsize = 10;
  char* result = nullptr;
  size = 0;

  char* batch = new char[batchsize];

  if (in.flags() & std::ios_base::skipws) {
    in >> std::noskipws;
  }

  char val;
  size_t i = 0;

  while (!(in >> val).fail() && val != '\n') {
    batch[i] = val;

    if (++i == batchsize) {
      i = 0;
      char* t;

      try {
        t = mergeStrings(result, batch, size, batchsize);
      } catch (...) {
        size = 0;
        delete[] result;
        delete[] batch;
        throw;
      }

      delete[] result;
      size += batchsize;
      result = t;
    }
  }

  if (in.fail() && !in.eof()) {
    delete[] batch;
    delete[] result;
    size = 0;
    return nullptr;
  }

  char* q;

  try {
    q = mergeStrings(result, batch, size, i);
  } catch (...) {
    size = 0;
    delete[] result;
    delete[] batch;
    throw;
  }

  delete[] result;
  size += i;
  result = q;

  delete[] batch;
  return result;
}

char* loseva::lat_two(const char* str1, const char* str2, char* result, size_t result_size)
{
  unsigned char letters[26] = {0};
  size_t res_index = 0;

  for (size_t i = 0; str1[i] != '0'; ++i) {
    char c = str1[i];
    if (std::isalpha(c)) {
      int idx = std::tolower(c) - 'a';
      if (!letters[idx]) {
        letters[idx] = 1;
      }
    }
  }

  for (size_t i = 0; str2[i] != '0'; ++i) {
    char c = str2[i];
    if (std::isalpha(c)) {
      int idx = std::tolower(c) - 'a';
      if (!letters[idx]) {
        letters[idx] = 1;
      }
    }
  }

  for (int i = 0; i < 26; ++i) {
    if (letters[i]) {
      if (res_index + 1 >= result_size) return nullptr;
      result[res_index++] = 'a' + i;
    }
  }
  result[res_index] = '0';
  return result;
}

char* loseva::lat_rmv(const char* src, char* result, size_t result_size)
{
  size_t res_index = 0;
  for (size_t i = 0; src[i] != '0'; ++i) {
    if (!std::isalpha(src[i])) {
      if (res_index + 1 >= result_size) return nullptr;
      result[res_index++] = src[i];
    }
  }
  result[res_index] = '0';
  return result;
}

int main()
{
  size_t size;
  char* input;

  try {
    input = loseva::readInputLine(std::cin, size);
  } catch (std::bad_alloc&) {
    std::cerr << "bad alloc\n";
    return 1;
  } catch (std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  if (!input) {
    std::cerr << "input error\n";
    return 1;
  }

  char result1[27];
  char* result2 = nullptr;

  try {
    result2 = new char[size + 1];
  } catch (std::bad_alloc&) {
    std::cerr << "bad alloc\n";
    delete[] input;
    return 1;
  }

  if (!loseva::lat_two(input, "def_ghk", result1, sizeof(result1))) {
    std::cerr << "output buffer too small for task 1\n";
    delete[] input;
    delete[] result2;
    return 1;
  }

  if (!loseva::lat_rmv(input, result2, size + 1)) {
    std::cerr << "output buffer too small for task 2\n";
    delete[] input;
    delete[] result2;
    return 1;
  }

  std::cout << result1 << "\n";
  std::cout << result2 << "\n";

  delete[] input;
  delete[] result2;

  return 0;
}
