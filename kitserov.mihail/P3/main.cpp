#include <iostream>
#include <fstream>

namespace kitserov
{
  int* createMatrix(size_t size);
  void variant1(size_t rows, size_t cols, int matrix[10000], const char* outputFile);
  void variant2(size_t rows, size_t cols, int* matrix, const char* outputFile);
}

int* kitserov::createMatrix(size_t size)
{
  if (size == 0) {
    return nullptr;
  }
  return new int[size];
}

void kitserov::variant1(size_t rows, size_t cols, int matrix[10000], const char* outputFile)
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
  
  std::ofstream output(outputFile);
  output << answer;
}

void kitserov::variant2(size_t rows, size_t cols, int* matrix, const char* outputFile)
{
  std::ofstream output(outputFile);
  if (rows != cols || rows == 0 || cols == 0) {
    output << "false";
    delete[] matrix;
    return;
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
  
  if (flag) {
    output << "true";
  } else {
    output << "false";
  }
  delete[] matrix;
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
    std::cerr << "Incorrect matrix dimensions\n";
    return 2;
  }
  
  if (num == 1) {
    int matrix[10000] = {0};
    for (size_t i = 0; i < rows * cols; i++) {
      input >> matrix[i];
      if (input.fail()) {
        std::cerr << "Failed to read matrix element\n";
        return 2;
      }
    }
    variant1(rows, cols, matrix, argv[3]);
  } else {
    int* matrix = createMatrix(rows * cols);
    if (matrix == nullptr) {
      std::cerr << "Failed to allocate memory for matrix\n";
      return 2;
    }
    
    for (size_t i = 0; i < rows * cols; i++) {
      input >> matrix[i];
      if (input.fail()) {
        std::cerr << "Failed to read matrix element\n";
        delete[] matrix;
        return 2;
      }
    }
    variant2(rows, cols, matrix, argv[3]);
  }
  
  return 0;
}
