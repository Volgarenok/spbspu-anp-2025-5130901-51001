// UNI-TWO && HAS-SAM new/delete

#include <iostream>
#include <istream>

namespace shaykhraziev {
  char * concat(char*a, char*b, size_t as, size_t bs) {
    size_t len = as + bs;
    char * result = new char[len];
    for (size_t i = 0; i < as; i++) {
      result[i] = a[i];
    }
    for (size_t i = 0; i < bs; i++) {
      result[as + i] = b[i];
    }
    return result;
  }

}

int main() {
  return 0;
}

