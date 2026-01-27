#include <iostream>
#include <fstream>

namespace smirnova {
  void task1(size_t rows, size_t cols, int* matrix, const char* outputFile);
  void task2(size_t rows, size_t cols, int* matrix, const char* outputFile);
}

bool isLocalMinimum(size_t i, size_t j, size_t cols, int* matrix) {
  int current = matrix[i * cols + j];
  return (current < matrix[(i - 1) * cols + j]) &&
         (current < matrix[(i + 1) * cols + j]) &&
         (current < matrix[i * cols + (j - 1)]) &&
         (current < matrix[i * cols + (j + 1)]);
}

size_t countLocalMin(size_t rows, size_t cols, int* matrix) {
  if (rows < 3 || cols < 3) return 0;
  
  size_t count = 0;
  for (size_t i = 1; i < rows - 1; i++) {
    for (size_t j = 1; j < cols - 1; j++) {
      if (isLocalMinimum(i, j, cols, matrix)) {
        count++;
      }
    }
  }
  return count;
}

void smirnova::task1(size_t rows, size_t cols, int* matrix, const char* outputFile) {
  size_t result = countLocalMin(rows, cols, matrix);
  std::ofstream out(outputFile);
  out << result;
}

void smirnova::task2(size_t rows, size_t cols, int* matrix, const char* outputFile) {
  if (rows == 0 || cols == 0) {
    std::ofstream out(outputFile);
    out << "0";
    return;
  }
  
  size_t maxRow = 0;
  size_t maxLen = 0;
  
  for (size_t i = 0; i < rows; i++) {
    size_t currLen = 1;
    size_t rowMax = 1;
    
    for (size_t j = 1; j < cols; j++) {
      if (matrix[i * cols + j] == matrix[i * cols + j - 1]) {
        currLen++;
      } else {
        if (currLen > rowMax) rowMax = currLen;
        currLen = 1;
      }
    }
    if (currLen > rowMax) rowMax = currLen;
    
    if (rowMax > maxLen) {
      maxLen = rowMax;
      maxRow = i;
    }
  }
  
  std::ofstream out(outputFile);
  out << (maxRow + 1);
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <1|2> <input> <output>\n";
    return 1;
  }
  
  int num = std::atoi(argv[1]);
  if (num != 1 && num != 2) {
    std::cerr << "First parameter must be 1 or 2\n";
    return 1;
  }
  
  std::ifstream in(argv[2]);
  if (!in) {
    std::cerr << "Cannot open input file\n";
    return 2;
  }
  
  size_t rows = 0, cols = 0;
  in >> rows >> cols;
  
  if (!in) {
    std::cerr << "Cannot read matrix dimensions\n";
    return 2;
  }
  
  if (rows == 0 && cols == 0) {
    std::ofstream out(argv[3]);
    out << "0";
    return 0;
  }
  
  const size_t MAX_SIZE = 10000;
  if (num == 1 && rows * cols > MAX_SIZE) {
    std::cerr << "Matrix too large for fixed array\n";
    return 2;
  }
  
  int* matrix = (num == 1) ? new int[MAX_SIZE] : new int[rows * cols];
  
  for (size_t i = 0; i < rows * cols; i++) {
    in >> matrix[i];
    if (!in) {
      delete[] matrix;
      std::cerr << "Cannot read matrix element\n";
      return 2;
    }
  }
  
  if (num == 1) {
    smirnova::task1(rows, cols, matrix, argv[3]);
  } else {
    smirnova::task2(rows, cols, matrix, argv[3]);
  }
  
  delete[] matrix;
  return 0;
}
