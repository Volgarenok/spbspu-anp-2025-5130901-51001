#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iomanip>

namespace hachaturyanov {
  char * readline(std::istream & in, size_t & strl);
  char * digits(const char * input, size_t & res);
  void exc_snd(const char * input, const char * second, const size_t second_len, char * output);
  void dgt_snd(const char * input, const char * second, const size_t second_len, char * output);
}

char * hachaturyanov::readline(std::istream & in, size_t & strl)
{
  size_t max_buffer_size = 512;
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws) {
    in >> std::noskipws;
  }

  char * input = reinterpret_cast< char* >(malloc(max_buffer_size));
  if (!input) {
    return nullptr;
  }

  size_t len = 0;

  bool done = false;
  while (!done) {
    for (size_t i = len; i < max_buffer_size - len; i++) {
      in >> input[i];
    }
    len = std::strlen(input);

    if (std::cin.fail() && !std::cin.eof()) {
      std::cin.clear();
      max_buffer_size *= 2;
      char * temp = reinterpret_cast< char* >(realloc(input, max_buffer_size));
      if (!temp) {
        free(input);
        return nullptr;
      }
      input = temp;
      continue;
    }
    done = true;
  }

  strl = len - 1;
  input[strl] = 0;

  if (is_skipws) {
    in >> std::skipws;
  }
  return input;
}

char * hachaturyanov::digits(const char * input, size_t & res)
{
  char * output = reinterpret_cast< char* >(malloc(std::strlen(input)));
  if (!output) {
    return nullptr;
  }

  size_t count = 0;
  for (size_t i = 0; i < std::strlen(input); i++) {
    if (std::isdigit(input[i])) {
      output[count] = input[i];
      count++;
    }
  }

  char * temp = reinterpret_cast< char* >(realloc(output, count));
  if (!temp) {
    free(output);
    return nullptr;
  }
  output = temp;
  res = count;
  return output;
}

void hachaturyanov::exc_snd(const char * input, const char * second, const size_t second_len, char * output)
{
  size_t out_len = 0;
  for (size_t i = 0; i < std::strlen(input); i++) {
    size_t count = 0;
    for (size_t j = 0; j < second_len; j++) {
      if (input[i]==second[j]) {
        break;
      }
      count++;
    }
    if (count == second_len) {
      output[out_len] = input[i];
      out_len++;
    }
  }
}

void hachaturyanov::dgt_snd(const char * input, const char * second, const size_t second_len, char * output)
{
  size_t out_len = 0;
  for (size_t i = 0; i < std::strlen(input); i++) {
    size_t count = 0;

  }
}


int main() {
  const char exc_snd_second[] = "abc";
  const char dgt_snd_second[] = "g1h2k";

  const size_t exc_snd_s_len = std::strlen(exc_snd_second);
  const size_t dgt_snd_s_len = std::strlen(dgt_snd_second);

  size_t strl = 0;
  char * input = hachaturyanov::readline(std::cin, strl);
  if (!input) {
    std::cerr << "Bad allocation\n";
    return 1;
  }

  char * outline1 = reinterpret_cast< char* >(malloc(strl));
  if (!outline1) {
    free(input);
    std::cerr << "Bad allocation\n";
    return 1;
  }

  size_t s_digits = 0;
  char * digits = hachaturyanov::digits(dgt_snd_second, s_digits);
  char * outline2 = reinterpret_cast< char* >(malloc(std::strlen(input) + s_digits));
  if (!outline2) {
    free(input);
    free(outline1);
    std::cerr << "Bad allocation\n";
    return 1;
  }

  hachaturyanov::exc_snd(input, exc_snd_second, exc_snd_s_len, outline1);
  std::cout << outline1 << "\n";
  free(input);
  free(outline1);
  return 0;
}
