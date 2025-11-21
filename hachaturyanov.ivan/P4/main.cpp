#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iomanip>

namespace hachaturyanov {
  char * readline(std::istream & in, size_t & strl);
  void exc_snd(char * input, char * second, size_t second_len, char * output);
}

char * hachaturyanov::readline(std::istream & in, size_t & strl) {
  size_t max_buffer_size = 512;
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws) {
    in >> std::noskipws;
  }

  char * input = (char*)malloc(max_buffer_size);
  if (!input) {
    return nullptr;
  }

  size_t len = 0;

  bool done = false;
  while (!done) {
    for (size_t i = len; i < max_buffer_size - len; i++) {
      in >> input[i];
    }
    len = strlen(input);

    if (std::cin.fail() && !std::cin.eof()) {
      std::cin.clear();
      max_buffer_size *= 2;
      char * temp = (char*)realloc(input, max_buffer_size);
      if (!temp) {
        free(input);
        return nullptr;
      }
      input = temp;
      continue;
    }
    done = true;
  }
  if (is_skipws) {
    in >> std::skipws;
  }
  return input;
}

void hachaturyanov::exc_snd(char * input, char * second, size_t second_len, char * output) {
  for (size_t i = 0; i < strlen(input); i++) {
    size_t count = 0;
    for (size_t j = 0; j < second_len; j++) {
      if (input[i]==second[j]) {
        break;
      }
    }
    output[i] = input[i];
  }
}

int main() {
  const char exc_snd_second[] = "abc";
  const char dgt_snd_second[] = "g1h2k";

  size_t exc_snd_s_len = strlen(exc_snd_second);
  size_t dgt_snd_s_len = strlen(dgt_snd_second);

  size_t strl = 0;
  char * input = hachaturyanov::readline(std::cin, strl);
  if (!input) {
    std::cerr << "Bad allocation\n";
    return 1;
  }

}
