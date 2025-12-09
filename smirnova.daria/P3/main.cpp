#include <iostream>
#include <fstream>

namespace smirnova
{
  void task1(size_t rows, size_t cols, int matrix, const char* outputFile);
  void task2(size_t rows, size_t cols, int* matrix, const char* outputFile);
}

bool isLocalMinimum(size_t i, size_t j, size_t cols, int* matrix)
{
  int current = matrix[i * cols + j];
  
  bool cond = true;
  cond = cond && (current < matrix[(i - 1) * cols + j]);
  cond = cond && (current < matrix[(i + 1) * cols + j]);
  cond = cond && (current < matrix[i * cols + (j - 1)]);
  cond = cond && (current < matrix[i * cols + (j + 1)]);
  
  return cond;
}

size_t countLocalMin(size_t rows, size_t cols, int* matrix)
{
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

void smirnova::task1(size_t rows, size_t cols, int* matrix, const char* outputFile)
{
  size_t result = countLocalMin(rows, cols, matrix);
  std::ofstream out(outputFile);
  out << result;
}

