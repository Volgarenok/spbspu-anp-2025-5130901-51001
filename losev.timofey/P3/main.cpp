#include <iostream>
#include <fstream>
#include <limits>

namespace losev {
  size_t readFile(std::ifstream & file, int * ptrArr, size_t m, size_t n);
  size_t findNumRowMaxRepl(const int * ptrArr, size_t m, size_t n);
  size_t countLocMin(const int * ptrArr, size_t m, size_t n);
}

int main(int argc, char ** argv)
{
  namespace los = losev;
  if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  } else if (argc < 4) {
    std::cerr << "Too few arguments\n";
    return 1;
  }
  if ((argv[1][0] != '1' && argv[1][0] != '2') || argv[1][1] != '\0') {
    std::cerr << "Error: first parameter must be exactly '1' or '2'\n";
    return 1;
  }
  std::ofstream output(argv[3]);
  if (!output.is_open()) {
    std::cerr << "I can not open output file\n";
    return 1;
  } else {
    output.clear();
  }
  std::ifstream input(argv[2]);
  size_t m = 0, n = 0;
  if (input.is_open()) {
    input >> m >> n;
    if (input.fail()) {
      std::cerr << "I can not read matrix size\n";
      return 2;
    }
    if (m == 0 && n == 0) {
      output << 0 << "\n" << 0 << " " << 0 << "\n";
      return 0;
    }
  } else {
    std::cerr << "I can not open input file\n";
    return 1;
  }
  int *arrayDinamic = nullptr;
  int arrayStack[10000];
  int *array = nullptr;
  if (*argv[1] == '1') {
    array = arrayStack;
  } else {
    arrayDinamic = new int[n * m];
    array = arrayDinamic;
  }
  size_t readCount = los::readFile(input, array, m, n);
  if (readCount != m * n) {
    std::cerr << "Error reading matrix\n";
    delete[] arrayDinamic;
    return 2;
  }
  input.close();
  output << los::countLocMin(array, m, n) << "\n";
  output << los::findNumRowMaxRepl(array, m, n) << "\n";
  delete[] arrayDinamic;
  return 0;
}

size_t losev::readFile(std::ifstream & file, int * ptrArr, size_t m, size_t n)
{
  int temp;
  size_t count = 0;
  for (size_t i = 0; i < m; i++) {
    for (size_t j = 0; j < n; j++) {
      file >> temp;
      if (file.fail()) {
        return count;
      }
      ptrArr[i * n + j] = temp;
      count++;
    }
  }
  return count;
}

size_t losev::countLocMin(const int * ptrArr, size_t m, size_t n)
{
  size_t count = 0;
  bool isLocMin;
  for (size_t i = 1; i < m - 1; i++) {
    for (size_t j = 1; j < n - 1; j++) {
      isLocMin = ptrArr[i * n + j] < ptrArr[i * n + (j + 1)];
      isLocMin = isLocMin && ptrArr[i * n + j] < ptrArr[(i + 1) * n + (j + 1)];
      isLocMin = isLocMin && ptrArr[i * n + j] < ptrArr[(i + 1) * n + j];
      isLocMin = isLocMin && ptrArr[i * n + j] < ptrArr[(i + 1) * n + (j - 1)];
      isLocMin = isLocMin && ptrArr[i * n + j] < ptrArr[i * n + (j - 1)];
      isLocMin = isLocMin && ptrArr[i * n + j] < ptrArr[(i - 1) * n + (j - 1)];
      isLocMin = isLocMin && ptrArr[i * n + j] < ptrArr[(i - 1) * n + j];
      isLocMin = isLocMin && ptrArr[i * n + j] < ptrArr[(i - 1) * n + (j + 1)];
      if (isLocMin) {
        count++;
      }
    }
  }
  return count;
}

size_t losev::findNumRowMaxRepl(const int * ptrArr, size_t m, size_t n)
{
  size_t numRowMaxsLenRep = 0;
  size_t maxLenRep = 0;
  for (size_t i = 0; i < m; i++) {
    size_t locMaxLenRep = 0;
    size_t lenRep = 0;
    for (size_t j = 0; j < n - 1; j++) {
      if (ptrArr[i * n + j] == ptrArr[i * n + j + 1]) {
        lenRep++;
        if (lenRep > locMaxLenRep) {
          locMaxLenRep = lenRep;
        }
      } else {
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
