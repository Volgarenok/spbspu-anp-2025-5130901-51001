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

  char * getline(std::istream &in, size_t &size) {
    size_t batchsize = 10;
    char * result = nullptr;
    size = 0;
    char *batch = new char[batchsize];

    if (in.flags() & std::ios_base::skipws) {
      in >> std::noskipws;
    }

    char val;
    size_t i = 0;
    while (!(in >> val).fail() && val != '\n') {
      batch[i] = val;

      if (++i == batchsize) {
        i = 0;
        char * t;
        try {
          t = concat(result, batch, size, batchsize);
        } catch (...) {
          size = 0;
          delete[] result;
          delete [] batch;
          throw;
        }
        delete [] result;
        size += batchsize;
        result = t;
      }
    }

    if (in.fail()) {
      delete[] batch;
      delete[] result;
      throw std::logic_error("input error");
    }

    char * q;
    try {
      q = concat(result, batch, size, i);
    } catch (...) {
      size = 0;
      delete[] result;
      delete [] batch;
      throw;
    }

    delete [] result;
    size += i;
    result = q;

    delete[] batch;
    return result;
  }
}

int main() {
  return 0;
}

