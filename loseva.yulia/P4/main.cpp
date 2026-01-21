#include <iostream>
#include <istream>
#include <cctype>
#include <new>

namespace loseva
{
  char* mergeStrings(const char* a, const char* b, size_t as, size_t bs);
  char* readInputLine(std::istream& in, size_t& size);
  char* lat_two(const char* str1, const char* str2, char* result, size_t result_size);
  char* lat_rmv(const char* src, char* result, size_t result_size);
}
char* loseva::mergeStrings(const char* a, const char* b, size_t as, size_t bs)
{
   char* result = new char[as + bs];
  if (a) {
    for (size_t i = 0; i < as; i++) result[i] = a[i];
  }
  if (b) {
    for (size_t i = 0; i < bs; i++) result[as + i] = b[i];
  }
  return result;
}
char* loseva::readInputLine(std::istream& in, size_t& size)
{
  const size_t batchsize = 10;
  char* result = nullptr;
  size = 0;
  char* batch = new char[batchsize];
  in >> std::noskipws;
  char val;
  size_t i = 0;
  try {
    while ((in >> val) && val != '\n') {
      batch[i++] = val;
      if (i == batchsize) {
        char* t = mergeStrings(result, batch, size, batchsize);
        delete[] result;
        result = t;
        size += batchsize;
        i = 0;
      }
    }
    char* final_res = new char[size + i + 1];
    if (result) {
      for (size_t j = 0; j < size; ++j) final_res[j] = result[j];
    }
    for (size_t j = 0; j < i; ++j) final_res[size + j] = batch[j];
        size += i;
    final_res[size] = '\0';
    delete[] result;
    delete[] batch;
    return final_res;
  } catch (const std::bad_alloc&) {
    delete[] result;
    delete[] batch;
    throw; 
  }
}
char* loseva::lat_two(const char* str1, const char* str2, char* result, size_t result_size)
{
  bool letters[26] = {false};
  size_t res_index = 0;
  auto process = [&](const char* s) {
    for (size_t j = 0; s[j] != '\0'; ++j) {
      if (std::isalpha(static_cast<unsigned char>(s[j]))) {
        letters[std::tolower(static_cast<unsigned char>(s[j])) - 'a'] = true;
      }
    }
  };
  process(str1);
  process(str2);
  for (int j = 0; j < 26; ++j) {
    if (letters[j]) {
      if (res_index + 1 >= result_size) return nullptr;
      result[res_index++] = static_cast<char>('a' + j);
    }
  }
  result[res_index] = '\0';
  return result;
}
char* loseva::lat_rmv(const char* src, char* result, size_t result_size)
{
  size_t res_index = 0;
  for (size_t j = 0; src[j] != '\0'; ++j) {
    if (!std::isalpha(static_cast<unsigned char>(src[j]))) {
      if (res_index + 1 >= result_size) return nullptr;
      result[res_index++] = src[j];
    }
  }
  result[res_index] = '\0';
  return result;
}
int main()
{
  char* input = nullptr;
  char* result2 = nullptr;
  size_t size = 0;
  try {
    input = loseva::readInputLine(std::cin, size);
    char result1[27];
    result2 = new char[size + 1];
    if (loseva::lat_two(input, "defghk", result1, sizeof(result1))) {
      std::cout << result1 << "\n";
    }
    if (loseva::lat_rmv(input, result2, size + 1)) {
      std::cout << result2 << "\n";
    }
  } catch (const std::bad_alloc&) {
    std::cerr << "bad alloc\n";
    delete[] input;
    delete[] result2;
    return 1;
  }
  delete[] input;
  delete[] result2;
  return 0;
}
