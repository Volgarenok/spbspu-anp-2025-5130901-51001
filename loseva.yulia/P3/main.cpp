#include <iostream>
#include <fstream>

namespace loseva
{
  void maxDiagonalSum(size_t rows, size_t cols, int* matrix);
  void countLocalMax(size_t rows, size_t cols, int* matrix);
}
bool isValidPosition(size_t i, size_t j, size_t rows, size_t cols)
{
  return i < rows && j < cols;
}

bool isLocalMaximum(size_t i, size_t j, size_t rows, size_t cols, int* matrix)
{
  int current = matrix[i * cols + j];
  int di[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int dj[] = {-1, 0, 1, -1, 1, -1, 0, 1};
  for (int k = 0; k < 8; ++k)
  {
    size_t ni = i + di[k];
    size_t nj = j + dj[k];
    if (isValidPosition(ni, nj, rows, cols) && matrix[ni * cols + nj] >= current)
    {
      return false;
    }
  }
  return true;
}

void loseva::maxDiagonalSum(size_t rows, size_t cols, int* matrix)
{
  if (rows == 0 || cols == 0) return 0; 

  size_t maxsum = 0;
  size_t n  = std::min(rows, cols);
  for (int sum = 0; sum <= 2 * (n - 1); ++sum)
 {
    if (sum == n - 1) continue;
    size_t diagsum = 0;
    for (size_t i = 0; i < n; ++i)
    {
      int j = sum - i;
      if ((j >= 0) && (j < n))
      {
        diagsum += matrix[i * cols + j];
      }
    }
    if (diagsum > maxsum)
    {
      maxsum = diagsum;
    }
  }
  return maxsum;
}

void loseva::countLocalMax(size_t rows, size_t cols, int** matrix)
{

  size_t count = 0;
  if (rows < 3 || cols < 3) return 0;
  for (size_t i = 1; i < rows - 1; i++) {
    for (size_t j = 1; j < cols - 1; j++) {
      if (isLocalMax(i, j, rows, cols, matrix)){
        ++count;
      }
    }
  }
  return count;
}

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }

  if (argc > 4)
  {
    std::cerr << "Too many arguments\n";
    return 1;
  }

  char* end;
  long num = std::strtol(argv[1], &end, 10);
  if (*end != '\0' ||  num < 1 ||  num > 2)
  {
    std::cerr << "First parameter is incorrect\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input.is_open())
  {
    std::cerr << "Cannot open input file\n";
    return 2;
  }

  size_t rows, cols;
  input >> rows >> cols;
  if (input.fail())
  {
    std::cerr << "Cannot read matrix dimensions\n";
    return 2;
  }
  int* matrix = nullptr;

  if (rows == 0 || cols == 0)
  {
    matrix = new int[0];
  }
  else
  {
    matrix = new int[rows * cols];
    for (size_t i = 0; i < rows * cols; i++)
    {
      input >> matrix[i];
      if (input.fail())
      {
        std::cerr << "Cannot read matrix element\n";
        delete[] matrix;
        return 2;
      }
    }
    size_t result = 0;
    if (num == 1)
    {
      result = loseva::maxDiagonalSum(rows, cols, matrix);
    }
    else
    {
      result = countLocalMax(rows, cols, matrix);
    }
    std::ofstream output(argv[3]);
    output << result;

    delete[] matrix;
    return 0;
}
