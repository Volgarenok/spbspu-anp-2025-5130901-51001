#include <iostream>
#include <istream>
#include <cstring>
#include <stdexcept>

namespace shaykhraziev
{
  char* concat(const char* a, const char* b, size_t as, size_t bs);
  char* getline(std::istream& in, size_t& size);
  void combineStrings(char* sum, const char* a, const char* b, size_t as, size_t bs);
  int hasSame(const char* a, const char* b);
}

char* shaykhraziev::concat(const char* a, const char* b, const size_t as, const size_t bs)
{
  const size_t len = as + bs;
  char* result = new char[len];

  for (size_t i = 0; i < as; i++) {
    result[i] = a[i];
  }

  for (size_t i = 0; i < bs; i++) {
    result[as + i] = b[i];
  }

  return result;
}

char* shaykhraziev::getline(std::istream& in, size_t& size)
{
  const size_t batch_size = 10;
  char* result = nullptr;
  size = 0;

  char* batch = new char[batch_size + 1];

  if (in.flags() & std::ios_base::skipws) {
    in >> std::noskipws;
  }

  char ch = ' ';
  size_t i = 0;

  while (!(in >> ch).fail() && ch != '\n') {
    batch[i] = ch;

    if (++i == batch_size) {
      i = 0;
      char* temp = nullptr;

      try {
        temp = concat(result, batch, size, batch_size);
      } catch (const std::bad_alloc& e) {
        delete[] result;
        delete[] batch;
        throw;
      }

      delete[] result;
      size += batch_size;
      result = temp;
    }
  }

  if (in.fail()) {
    delete[] batch;
    delete[] result;
    throw std::logic_error("input failed");
  }

  char* temp = nullptr;
  batch[i] = '\0';

  try {
    temp = concat(result, batch, size, i + 1);
  } catch (const std::bad_alloc& e) {
    delete[] result;
    delete[] batch;
    throw;
  }

  delete[] result;
  size += i;
  delete[] batch;

  return temp;
}

void shaykhraziev::combineStrings(char* sum, const char* a, const char* b, size_t as, size_t bs)
{
  size_t common_length = 0;
  size_t diff_length = 0;
  const char* tail = nullptr;

  if (as >= bs) {
    common_length = bs;
    diff_length = as - bs;
    tail = a;
  } else {
    common_length = as;
    diff_length = bs - as;
    tail = b;
  }

  for (size_t i = 0; i < common_length; i++) {
    sum[2 * i] = a[i];
    sum[2 * i + 1] = b[i];
  }

  for (size_t i = 0; i < diff_length; i++) {
    sum[2 * common_length + i] = tail[common_length + i];
  }
}

int shaykhraziev::hasSame(const char* a, const char* b)
{
  size_t freq[256] = {};

  for (size_t i = 0; a[i] != '\0'; i++) {
    freq[static_cast< unsigned char >(a[i])] += 1;
  }

  for (size_t i = 0; b[i] != '\0'; i++) {
    if (freq[static_cast< unsigned char >(b[i])] > 0) {
      return 1;
    }
  }

  return 0;
}

int main()
{
  char* input1 = nullptr;
  const char* input2 = "QWERTYUIOP\0";
  size_t input1_length = 0;

  try {
    input1 = shaykhraziev::getline(std::cin, input1_length);
  } catch (const std::bad_alloc& e) {
    std::cerr << "bad alloc " << e.what() << "\n";
    return 1;
  } catch (const std::logic_error& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  const size_t input2_length = std::strlen(input2);
  const size_t total_size = input1_length + input2_length;
  char* sum = nullptr;

  if (total_size == 0) {
    std::cerr << "nothing working with\n";
    delete[] input1;
    return 1;
  }

  try {
    sum = new char[total_size + 1];
    sum[total_size] = '\0';
  } catch (const std::bad_alloc& e) {
    std::cerr << "bad alloc " << e.what() << "\n";
    delete[] input1;
    return 1;
  }

  shaykhraziev::combineStrings(sum, input1, input2, input1_length, input2_length);
  std::cout << sum << "\n";

  int has_same = shaykhraziev::hasSame(input1, input2);
  std::cout << has_same << "\n";

  delete[] sum;
  delete[] input1;

  return 0;
}
