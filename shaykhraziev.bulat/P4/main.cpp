#include <iostream>
#include <istream>
#include <cstring>

namespace shaykhraziev
{
  char* concat(const char* a, const char* b, size_t as, size_t bs);
  char* getline(std::istream& in);
  void combineStrings(char* sum, const char* a, const char* b);
  int hasSame(const char* a, const char* b);
}

char* shaykhraziev::concat(const char* a, const char* b, const size_t as, const size_t bs)
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

char* shaykhraziev::getline(std::istream& in)
{
  size_t batchsize = 10;
  char* result = nullptr;
  size_t size = 0;

  char* batch = new char[batchsize + 1];

  if (in.flags() & std::ios_base::skipws) {
    in >> std::noskipws;
  }

  char val = ' ';
  size_t i = 0;

  while (!(in >> val).fail() && val != '\n') {
    batch[i] = val;

    if (++i == batchsize) {
      i = 0;
      char* temp = nullptr;

      try {
        temp = concat(result, batch, size, batchsize);
      } catch (const std::bad_alloc& e) {
        delete[] result;
        delete[] batch;
        throw;
      }

      delete[] result;
      size += batchsize;
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
    temp = concat(result, batch, size, i+1);
  } catch (const std::bad_alloc& e) {
    delete[] result;
    delete[] batch;
    throw;
  }

  delete[] result;
  delete[] batch;
  return temp;
}

void shaykhraziev::combineStrings(char* sum, const char* a, const char* b)
{
  bool flag1 = true, flag2 = true;
  size_t i = 0;
  while (flag1 && flag2) {
    if (i % 2 == 0) {
      char c = a[i/2];
      if (c == '\0') {
        flag1 = false;
        continue;
      }
      sum[i] = c;
    }
    if (i % 2 != 0) {
      char c = b[i/2];
      if (c == '\0') {
        flag2 = false;
        continue;
      }
      sum[i] = c;
    }
    i++;
  }

  const size_t j = i/2;
  size_t cnt = 0;
  while (flag1 && a[j+cnt] != '\0') {
    sum[i+cnt] = a[j+cnt];
    cnt++;
  }

  while (flag2 && b[j+cnt] != '\0') {
    sum[i+cnt] = b[j+cnt];
    cnt++;
  }

  // sum[i+cnt] = '\0';
}

int shaykhraziev::hasSame(const char* a, const char* b)
{
  size_t freq[256] = {};

  for (size_t i = 0; a[i] != '\0'; i++) {
    freq[static_cast<unsigned char> (a[i])]++;
  }

  for (size_t i = 0; b[i] != '\0'; i++) {
    if (freq[static_cast<unsigned char> (b[i])] > 0) {
      return 1;
    }
  }

  return 0;
}

int main() {
  char* inp1 = nullptr;
  const char* inp2 = "\0";

  try {
    inp1 = shaykhraziev::getline(std::cin);
  } catch (const std::bad_alloc& e) {
    std::cerr << "bad alloc " << e.what() <<"\n";
    return 1;
  } catch (const std::logic_error& e) {
    std::cerr << e.what() <<"\n";
    return 1;
  }

  size_t inp1len = strlen(inp1), inp2len = strlen(inp2);

  size_t size = inp1len + inp2len;
  char* sum;

  if (size == 0) {
    std::cerr << "nothing working with\n";
    delete[] inp1;
    return 1;
  }

  try {
    sum = new char[size];
  } catch (const std::bad_alloc& e) {
    std::cerr << "bad alloc " << e.what() <<"\n";
    delete[] inp1;
    return 1;
  }

  shaykhraziev::combineStrings(sum, inp1, inp2);
  std::cout << sum << "\n";
  delete[] sum;

  int hasSam = shaykhraziev::hasSame(inp1, inp2);
  std::cout << hasSam << "\n";

  delete[] inp1;

  return 0;
}
