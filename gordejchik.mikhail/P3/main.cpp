#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>

namespace gordejchik
{
  bool readMatrix(std::ifstream& inputFile, int* matrix, size_t size) {
    int num;
    size_t c = 0;
    while (inputFile >> num) {
      matrix[c] = num;
      c++;
    }
    if (inputFile.fail() && !inputFile.eof()) {
      std::cerr << "Error input\0";
      return false;
    }
    if (size != c) {
      std::cerr << "Incorrect number of parameters\0";
      return false;
    }
    return true;
  }
}
