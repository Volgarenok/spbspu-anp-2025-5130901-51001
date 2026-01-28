#include "task_variants.hpp"
#include <algorithm>

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
