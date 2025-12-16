#include <iostream>
#include <fstream>
#include <cstdlib>
namespace smirnova
{
  size_t findLocalMaximum(size_t rows, size_t cols, const int *matrix);
  int findElementsDiagonal(size_t rows, size_t cols, const int *matrix);
}
size_t smirnova::findLocalMaximum(size_t rows, size_t cols, const int *matrix)
{
  size_t count = 0;
  int d[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
  for (size_t i = 1; i < rows - 1; ++i)
  {
    for (size_t j = 1; j < cols - 1; ++j)
    {
      int val = matrix[i * cols + j];
      bool flag = true;
      for (size_t k = 0; k < 8 && flag; ++k)
      {
        size_t ni = i + d[k][0];
        size_t nj = j + d[k][1];
        if (matrix[ni * cols + nj] >= val)
        {
          flag = false;
        }
      }
      if (flag)
      {
        ++count;
      }
    }
  }
  return count;
}
int smirnova::findElementsDiagonal(size_t rows, size_t cols, const int *matrix)
{
  size_t n = std::min(rows, cols);
  if (n == 0)
  {
    return 0;
  }
  int maxsum = matrix[0];
  for (size_t i = 0; i < n; ++i)
  {
    for (size_t j = 0; j < n; ++j)
    {
      if (i == 0 && j == 0) {
        continue;
      }
      size_t x = i, y = j;
      int diagsum = 0;
      while (x < n && y < n)
      {
        diagsum += matrix[x * cols + y];
        ++x;
        ++y;
      }
      maxsum = std::max(diagsum,maxsum);
    }
  }
  return maxsum;
}
int main(int argc, char** argv)
{
  if (argc != 4)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  char* arg = argv[1];
  if ((arg[0] != '1' && arg[0] != '2') || arg[1] != '\0')
  {
    std::cerr << "Garbage in argument 1\n";
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input.is_open())
  {
    std::cerr << "Cannot open input file\n";
    return 1;
  }
  const size_t BUF = 10000;
  size_t rows = 0, cols = 0;
  if (!(input >> rows >> cols) || rows * cols > BUF)
  {
    std::cerr << "Error: invalid matrix size\n";
    return 2;
  }
  if (rows == 0 || cols == 0)
  {
    std::ofstream output(argv[3]);
    if (!output.is_open()) {
      std::cerr << "Cannot open output file\n";
      return 1;
    }
    output << "0 0\n0\n";
    return 0;
  }
  int *matrix = nullptr;
  int fixedMatrix[BUF] = {};
  if (arg[0] == '2')
  {
    matrix = new int[rows * cols];
  }
  else
  {
    matrix = fixedMatrix;
  }
  for (size_t i = 0; i < rows * cols; ++i)
  {
    if (!(input >> matrix[i]))
    {
      std::cerr << "Error: invalid element or out of range\n";
      if (arg[0] == '2')
      {
        delete[] matrix;
      }
      return 1;
    }
  }
  char extra = '\0';
  if (input >> extra)
  {
    std::cerr << "Error: extra data in input file (e.g., letter '" << extra << "')\n";
    return 2;
  }
  input.close();
  std::ofstream output(argv[3]);
  if (!output.is_open()) {
    std::cerr << "Cannot open output file\n";
    if (arg[0] == '2')
    {
      delete[] matrix;
    }
    return 1;
  }
  output << smirnova::findLocalMaximum(rows, cols, matrix) << "\n";
  output << smirnova::findElementsDiagonal(rows, cols, matrix) << "\n";
  if (arg[0] == '2')
  {
    delete[] matrix;
  }
  return 0;
}
