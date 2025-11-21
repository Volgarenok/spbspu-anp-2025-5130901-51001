// UNI-TWO && HAS-SAM new/delete

#include <iostream>
#include <istream>

namespace shaykhraziev {
  char * concat(char*a, char*b, size_t as, size_t bs);
  char * getline(std::istream &in, size_t &size);
  void uni_two(char * sum, char * a, char * b, size_t as, size_t bs);
  int has_sam(char * a, char * b, size_t as, size_t bs);
  void printarray(char * a, size_t s);
}

char * shaykhraziev::concat(char*a, char*b, size_t as, size_t bs) {
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

char * shaykhraziev::getline(std::istream &in, size_t &size) {
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

void shaykhraziev::uni_two(char * sum, char * a, char * b, size_t as, size_t bs) {
  size_t cmn, odd;
  char * t;
  if (as >= bs) {
    cmn = bs;
    odd = as - bs;
    t = a;
  } else {
    cmn = as;
    odd = bs - as;
    t = b;
  }

  for (size_t i = 0; i < cmn; i++) {
    sum[2*i] = a[i];
    sum[2*i+1] = b[i];
  }

  for (size_t i = 0; i < odd; i++) {
    sum[2*cmn + i] = t[cmn + i];
  }

}

int shaykhraziev::has_sam(char * a, char * b, size_t as, size_t bs) {
  size_t freq[256] = {};

  for (size_t i = 0; i < as; i++) {
    freq[a[i]]++;
  }

  for (size_t i = 0; i < bs; i++) {
    if (freq[b[i]] > 0) {
      return 1;
    }
  }

  return 0;
}


void shaykhraziev::printarray(char * a, size_t s) {
  for (size_t i = 0; i < s; i++) {
    std::cout << a[i];
  }
  std::cout << "\n";
}



int main() {
  size_t s1, s2;
  char * inp1, * inp2;

  try {
    inp1 = shaykhraziev::getline(std::cin, s1);
  } catch (std::bad_alloc &) {
    std::cerr << "bad alloc" << "\n";
    return 1;
  } catch (std::exception &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  try {
    inp2 = shaykhraziev::getline(std::cin, s2);
  } catch (std::bad_alloc &) {
    std::cerr << "bad alloc" << "\n";
    delete [] inp1;
    return 1;
  } catch (std::exception &e) {
    std::cerr << e.what() << "\n";
    delete [] inp1;
    return 1;
  }

  //uni-two
  size_t size = s1 + s2;
  char * sum;
  try {
    sum = new char[size];
  } catch (std::bad_alloc &) {
    std::cerr << "bad alloc" << "\n";
    delete [] inp1;
    delete [] inp2;
    return 1;
  }

  shaykhraziev::uni_two(sum, inp1, inp2, s1, s2);
  shaykhraziev::printarray(sum, size);
  delete[] sum;

  //has-sam
  int hasSam = shaykhraziev::has_sam(inp1, inp2, s1, s2);
  std::cout << hasSam << "\n";

  delete[] inp1;
  delete[] inp2;
  return 0;
}

