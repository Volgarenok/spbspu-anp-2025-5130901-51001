#include <iostream>
#include <fstream>
#include <limits>

namespace losev {
  void readFile(std::ifstream & file, int * ptrArr, size_t m, size_t n);
  int numRowLSR(int * ptrArr, int m, int n);
  int countLocMin(int * ptrArr, size_t m, size_t n);
}

int main(int argc, char **argv) {
  namespace los = losev;
  if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  else if (argc < 4) {
    std::cerr << "Too few arguments\n";
    return 1;
  }
  std::ofstream output(argv[3]);
  if (!output.is_open()) {
    std::cerr <<"I can not open output file\n";
    return 1;
  }
  else {
    output.clear();
  }
  std::ifstream input(argv[2]);
  int m = -1, n = -1;
  if (input.is_open()) {
    input >> m >> n;
    if (input.fail()) {
        std::cerr << "I can not read matrix size\n";
        return 2;
    }
    if (m < 0 || n < 0) {
      std::cerr << "I can not create an array from this\n";
      return 2;
    }
    else if (m == 0 && n == 0) {
      output << 0 << std::endl << 0 << " " << 0 << std::endl;
      return 0;
    }
  }
  else {
    std::cerr <<"I can not open input file\n";
    return 1;
  }
  int *arrayDinamic = nullptr;
  int arrayStack[10000];
  int *array = nullptr;
  if (*argv[1] == '1') {
    array = arrayStack;
  }
  else if (*argv[1] == '2') {
    arrayDinamic = new int[n * m];
    array = arrayDinamic;
  }
  else {
    std::cerr << "Error wrong num\n";
    return 1;
  }
  try {
    los::readFile(input, array, m, n);
  }
  catch (const std::runtime_error& e) {
    std::cerr << "Error file: " << e.what() << std::endl;
    return 2;
  }
  input.close();
  output << los::countLocMin(array, m, n) << std::endl;
  output << los::numRowLSR(array, m, n) << std::endl;
  delete[] arrayDinamic;
  return 0;
}

void losev::readFile(std::ifstream & file, int * ptrArr, size_t m, size_t n) {
  long long temp;
  for (size_t i = 0; i < m; i++) {
    for (size_t j = 0; j < n; j++) {
      file >> temp;
      if (file.fail()) {
        throw std::runtime_error("I can not read matrix");
      }
      if (temp > std::numeric_limits<int>::max()) {
        throw std::runtime_error("Number is too large for int type");
      }
      ptrArr[i * n + j] = (int)temp;
    }
  }
}

int losev::countLocMin(int * ptrArr, size_t m, size_t n) {
  int count = 0;
  bool isLocMin;
  for (size_t i = 1; i < m - 1; i++) {
    for (size_t j = 1; j < n - 1; j++) {
      isLocMin = ptrArr[i * n + j] < ptrArr[i * n + (j + 1)] &&
                  ptrArr[i * n + j] < ptrArr[(i + 1) * n + (j + 1)] &&
                  ptrArr[i * n + j] < ptrArr[(i + 1) * n + j] &&
                  ptrArr[i * n + j] < ptrArr[(i + 1) * n + (j - 1)] &&
                  ptrArr[i * n + j] < ptrArr[i * n + (j - 1)] &&
                  ptrArr[i * n + j] < ptrArr[(i - 1) * n + (j - 1)] &&
                  ptrArr[i * n + j] < ptrArr[(i - 1) * n + j] &&
                  ptrArr[i * n + j] < ptrArr[(i - 1) * n + (j + 1)];
      if (isLocMin) {
        count++;
      }
    }
  }
  return count;
}

int losev::numRowLSR(int * ptrArr, int m, int n) {
  int numRowMaxsLenRep = 0;
  int maxLenRep = 0;
  for (int i = 0; i < m; i++) {
    int locMaxLenRep = 0;
    int lenRep = 0;
    for (int j = 0; j < n - 1; j++) {
      if (ptrArr[i * n + j] == ptrArr[i * n + j + 1]) {
        lenRep++;
        if (lenRep > locMaxLenRep) {
          locMaxLenRep = lenRep;
        }
      }
      else {
        lenRep = 0;
      }
      if (locMaxLenRep > maxLenRep) {
        numRowMaxsLenRep = i;
        maxLenRep = locMaxLenRep;
      }
    }
  }
  return numRowMaxsLenRep + 1;
}
