#include <iostream>
#include <fstream>

namespace kitserov
{
  size_t countRowsWithoutSame(size_t rows, size_t cols, int* matrix);
  bool isUpTriangleMatrix(size_t rows, size_t cols, int* matrix);
  bool isDigit(const char* str);
}

size_t kitserov::countRowsWithoutSame(size_t rows, size_t cols, int* matrix)
{
  size_t answer = 0;
  bool flag = false;

  for (size_t i = 0; i < rows; i++) {
    flag = false;
    for (size_t j = 0; j < (cols - 1); j++) {
      if (matrix[i * cols + j] == matrix[i * cols + j + 1]) {
        flag = true;
        break;
      }
    }
    if (!flag) {
      answer++;
    }
  }

  return answer;
}

bool kitserov::isDigit(const char* str) {
  if (str == nullptr || *str == '\0') {
    return false;
  }
  if (!std::isdigit(static_cast< unsigned char >(str[0]))) {
    return false;
  }
  if (!(str[1] == '\0')) {
    return false;
  }
  return true;
}

bool kitserov::isUpTriangleMatrix(size_t rows, size_t cols, int* matrix)
{
  if (rows != cols || rows == 0 || cols == 0) {
    return false;
  }

  bool flag = true;
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < i; j++) {
      if (matrix[i * cols + j] != 0) {
        flag = false;
        break;
      }
    }
    if (!flag) {
      break;
    }
  }

  return flag;
}

int main(int argc, char* argv[])
{
  using namespace kitserov;

  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }

  if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }

  if (!isDigit(argv[1])) {
    std::cerr << "First argument is not a digit\n";
    return 1;
  }
  int num = std::atoi(argv[1]);
  if (num != 1 && num != 2) {
    std::cerr << "First argument out incorrect (not 1 or 2)\n";
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cerr << "Failed to open input file\n";
    return 2;
  }

  size_t rows = 0;
  size_t cols = 0;
  input >> rows >> cols;

  if (input.fail()) {
    std::cerr << "Failed to read matrix dimensions\n";
    return 2;
  }

  if (rows == 0 || cols == 0) {
    return 0;
  }
  int fixedSizeMatrix[10000];
  int* matrix = nullptr;
  if (num == 1) {
    matrix = fixedSizeMatrix;
  } else {
    try {
      matrix = new int[rows * cols];
    } catch (const std::bad_alloc& e) {
      std::cerr << "Error allocating memory: " << e.what() << "\n";
      return 2;
    }
  }

  for (size_t i = 0; i < rows * cols; i++) {
    input >> matrix[i];
    if (!input.good()) {
      if (num == 2) {
        delete[] matrix;
      }
      std::cerr << "Error reading matrix\n";
      return 2;
    }
  }

  input.close();
  std::ofstream output(argv[3]);
  size_t result1 = countRowsWithoutSame(rows, cols, matrix);
  bool result2 = isUpTriangleMatrix(rows, cols, matrix);
  output << result1 << '\n';
  output << std::boolalpha << result2 << '\0';
  if (num == 2) {
    delete[] matrix;
  }
  return 0;
}
