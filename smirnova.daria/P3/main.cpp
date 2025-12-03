#include <iostream>
#include <fstream>

namespace smirnova
{
  void task1(size_t rows, size_t cols, int matrix[10000], const char* outputFile);
  void task2(size_t rows, size_t cols, int* matrix, const char* outputFile);
}

void smirnova::task1(size_t rows, size_t cols, int matrix[10000], const char* outputFile)
{
  size_t count = 0;
  for (size_t i = 1; i < rows - 1; i++)
    for (size_t j = 1; j < cols - 1; j++) {
      int current = matrix[i * cols + j];
      if (current < matrix[(i-1)*cols+j] && current < matrix[(i+1)*cols+j] &&
          current < matrix[i*cols+(j-1)] && current < matrix[i*cols+(j+1)])
        count++;
    }
  std::ofstream(outputFile) << count;
}

void smirnova::task2(size_t rows, size_t cols, int* matrix, const char* outputFile)
{
  size_t maxSeries = 0, resultRow = 0;
  for (size_t i = 0; i < rows; i++) {
    size_t current = 1, maxRow = 1;
    for (size_t j = 1; j < cols; j++) {
      current = (matrix[i*cols+j] == matrix[i*cols+j-1]) ? current + 1 : 1;
      if (current > maxRow) maxRow = current;
    }
    if (maxRow > maxSeries) {
      maxSeries = maxRow;
      resultRow = i;
    }
  }
  std::ofstream(outputFile) << resultRow;
  delete[] matrix;
}

int main(int argc, char* argv[])
{
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }

  if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }

  char* end;
  long num = std::strtol(argv[1], &end, 10);
  if (*end != '\0' || num < 1 || num > 2) {
    std::cerr << "First parameter is incorrect\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cerr << "Cannot open input file\n";
    return 2;
  }

  size_t rows, cols;
  input >> rows >> cols;
  if (input.fail()) {
    std::cerr << "Cannot read matrix dimensions\n";
    return 2;
  }

  if (rows == 0 || cols == 0) {
    std::ofstream(argv[3]) << 0;
    return 0;
  }

  if (num == 1) {
    if (rows * cols > 10000) {
      std::cerr << "Matrix too large for fixed array\n";
      return 2;
    }
    int matrix[10000];
    for (size_t i = 0; i < rows * cols; i++) {
      input >> matrix[i];
      if (input.fail()) {
        std::cerr << "Cannot read matrix element\n";
        return 2;
      }
    }
    smirnova::task1(rows, cols, matrix, argv[3]);
  } else {
    int* matrix = new int[rows * cols];
    for (size_t i = 0; i < rows * cols; i++) {
      input >> matrix[i];
      if (input.fail()) {
        std::cerr << "Cannot read matrix element\n";
        delete[] matrix;
        return 2;
      }
    }
    smirnova::task2(rows, cols, matrix, argv[3]);
  }

  return 0;
}
