#include <iostream>
#include <cctype>
#include <cstdlib>

namespace hachaturyanov {
  size_t max_buffer_size = 1024;
}

int main() {
  const char * exc_snd_second = "abc";
  const char * dgt_snd_second = "g1h2k";

  char * input = (char*)malloc(hachaturyanov::max_buffer_size);
  if (!input) {
    std::cerr << "Bad allocation\n";
    return 1;
  }

  char * output = (char*)malloc(hachaturyanov::max_buffer_size);
  if (!output) {
    std::cerr << "Bad allocation\n";
    return 1;
  }

}
