#include <iostream>
#include <fstream>

namespace kitserov
{
  int* createMatrix(int num, size_t rows, size_t cols);
  size_t countRowsWithoutSame(size_t rows, size_t cols, int* matrix);
  bool isUpTriangleMatrix(size_t rows, size_t cols, int* matrix);
}

int* kitserov::createMatrix(int num, size_t rows, size_t cols)
{
  int* matrix = nullptr;
  if (num == 1) {
    int fixedLen[10000];
    matrix = fixedLen;
  } else {
    int* dinamicMatrix = new int[rows * cols];
    matrix = dinamicMatrix;
  }
  return matrix;
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

  int num = std::atoi(argv[1]);
  if (num != 1 && num != 2) {
    std::cerr << "First argument incorrect\n";
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
  
  int* matrix = nullptr;
  matrix = createMatrix(num, rows, cols);
  if (matrix == nullptr) {
    std::cerr << "Error allocating memory\n";
    return 2;
  }
  
  for (size_t i = 0; i < rows * cols; i++) {
    input >> matrix[i];
    if (!input.good()) {
      input.close();
      delete[] matrix;
      std::cerr << "Error reading matrix\n";
      return 2;
    }
  }
  
  input.close();
  std::ofstream output(argv[3]);
  
  if (num == 1) {
    size_t result = countRowsWithoutSame(rows, cols, matrix);
    output << result;
  } else {
    bool result = isUpTriangleMatrix(rows, cols, matrix);
    output << (result ? "true" : "false");
    delete[] matrix;
  }

  return 0;
}
