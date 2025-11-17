#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstring>

namespace hachaturyanov {
  char * readline(char * buf, size_t * buf_size);
  void exc_snd(char * input, char * second, size_t second_len, char * output);
}

char * hachaturyanov::readline(char * buf, size_t * buf_size) {
  size_t len = 0;

  bool done = false;
  while (!done) {
    std::cin.getline(buf + len, *buf_size - len);
    len = strlen(buf);

    if (std::cin.fail() && !std::cin.eof()) {
      std::cin.clear();
      *buf_size *= 2;
      char * temp = (char*)realloc(buf, *buf_size);
      if (!temp) {
        free(buf);
        return nullptr;
      }
      buf = temp;
      continue;
    }
    done = true;
  }
  return buf;
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
  size_t max_buffer_size = 512;

  const char exc_snd_second[] = "abc";
  const char dgt_snd_second[] = "g1h2k";

  size_t exc_snd_s_len = strlen(exc_snd_second);
  size_t dgt_snd_s_len = strlen(dgt_snd_second);

  char * input = (char*)malloc(max_buffer_size);
  if (!input) {
    std::cerr << "Bad allocation\n";
    return 1;
  }

  input = hachaturyanov::readline(input, &max_buffer_size);
  if (!input) {
    std::cerr << "Too long input\n";
    return 1;
  }

}
