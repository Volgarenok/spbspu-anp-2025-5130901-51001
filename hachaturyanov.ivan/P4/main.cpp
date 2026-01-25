#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iomanip>

namespace hachaturyanov {
  char * readline(std::istream & in, size_t & strl);
  size_t digits(const char * input);
  void excludeSymbols(const char * input, const char * second, const size_t second_len, char * output);
  void addDigits(const char * input, const char * second, const size_t second_len, char * output);
}

char * hachaturyanov::readline(std::istream & in, size_t & strl)
{
  size_t max_buffer_size = 512;
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws) {
    in >> std::noskipws;
  }

  char * input = reinterpret_cast< char* >(malloc(sizeof(char) * max_buffer_size));
  if (!input) {
    if (is_skipws) {
      in >> std::skipws;
    }
    return nullptr;
  }

  size_t len = 0;
  char c = in.get();
  if (in.eof() || c == '\n') {
    char * temp = reinterpret_cast< char* >(malloc(sizeof(char)));
    if (!temp) {
      free(input);
      if (is_skipws) {
        in >> std::skipws;
      }
      return nullptr;
    }
    for (size_t i = 0; i < sizeof(char); i++) {
      temp[i] = input[i];
    }
    free(input);
    input = temp;
    input[0] = '\0';
    strl = 0;
    if (is_skipws) {
      in >> std::skipws;
    }
    return input;
  }

  bool done = false;
  while (!done) {
    char c = in.get();
    if (in.eof() || c == '\n') {
      break;
    }

    if (len + 1 >= max_buffer_size) {
      max_buffer_size *= 2;
      char * temp = reinterpret_cast< char* >(malloc(sizeof(char) * max_buffer_size));
      if (!temp) {
        free(input);
        if (is_skipws) {
          in >> std::skipws;
        }
        return nullptr;
      }
      for (size_t i = 0; i < sizeof(char) * max_buffer_size; i++) {
        temp[i] = input[i];
      }
      free(input);
      input = temp;
    }
    input[len] = c;
    len++;
  }

  strl = len;
  input[strl] = '\0';

  if (is_skipws) {
    in >> std::skipws;
  }
  return input;
}

size_t hachaturyanov::digits(const char * input)
{
  size_t count = 0;
  for (size_t i = 0; i < std::strlen(input); i++) {
    if (std::isdigit(static_cast< unsigned char >(input[i]))) {
      count++;
    }
  }
  return count;
}

void hachaturyanov::excludeSymbols(const char * input, const char * second, const size_t second_len, char * output)
{
  size_t out_len = 0;
  for (size_t i = 0; input[i] != '\0'; i++) {
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
  output[out_len] = '\0';
}

void hachaturyanov::addDigits(const char * input, const char * second, const size_t second_len, char * output)
{

  for (size_t i = 0; input[i] != '\0'; i++) {
    output[i] = input[i];
  }

  size_t count = std::strlen(input);
  for (size_t i = 0; i < second_len; i++) {
    if (std::isdigit(static_cast< unsigned char >(second[i]))) {
      output[count] = second[i];
      count++;
    }
  }
  output[count] = '\0';
}


int main()
{
  const char exc_snd_second[] = "abc";
  const char dgt_snd_second[] = "g1h2k";

  const size_t exc_snd_s_len = std::strlen(exc_snd_second);
  const size_t dgt_snd_s_len = std::strlen(dgt_snd_second);

  size_t strl = 0;
  char * input = hachaturyanov::readline(std::cin, strl);
  if (!input) {
    std::cerr << "Bad allocation\n";
    return 1;
  } else if (!std::strlen(input)) {
    free(input);
    std::cerr << "Empty string\n";
    return 1;
  }

  char * outline1 = reinterpret_cast< char* >(malloc(sizeof(char) * (strl+ 1)));
  if (!outline1) {
    free(input);
    std::cerr << "Bad allocation\n";
    return 1;
  }

  size_t s_digits = hachaturyanov::digits(dgt_snd_second);
  char * outline2 = reinterpret_cast< char* >(malloc(sizeof(char) * (std::strlen(input) + s_digits + 1)));
  if (!outline2) {
    free(input);
    free(outline1);
    std::cerr << "Bad allocation\n";
    return 1;
  }

  hachaturyanov::excludeSymbols(input, exc_snd_second, exc_snd_s_len, outline1);
  hachaturyanov::addDigits(input, dgt_snd_second, dgt_snd_s_len, outline2);
  std::cout << outline1 << "\n";
  std::cout << outline2 << "\n";
  free(input);
  free(outline1);
  free(outline2);
  return 0;
}
