#include "matrix_transformations.hpp"
#include <cstddef>

void karpenko::transformMatrixSpiral(std::size_t rows, std::size_t cols, int matrix[])
{
  if (rows == 0 || cols == 0)
  {
    return;
  }

  std::size_t top = 0;
  std::size_t bottom = rows - 1;
  std::size_t left = 0;
  std::size_t right = cols - 1;
  int counter = 1;

  while (top <= bottom && left <= right)
  {
    for (std::size_t i = top; i <= bottom; ++i)
    {
      matrix[i * cols + left] += counter++;
    }
    ++left;

    for (std::size_t i = left; i <= right; ++i)
    {
      matrix[bottom * cols + i] += counter++;
    }
    --bottom;

    if (left <= right)
    {
      for (std::size_t i = bottom + 1; i > top; --i)
      {
        matrix[(i - 1) * cols + right] += counter++;
      }
      --right;
    }

    if (top <= bottom)
    {
      for (std::size_t i = right + 1; i > left; --i)
      {
        matrix[top * cols + (i - 1)] += counter++;
      }
      ++top;
    }
  }
}

void karpenko::createSmoothedMatrix(std::size_t rows, std::size_t cols, const int matrix[], double smoothed[])
{
  if (rows == 0 || cols == 0)
  {
    return;
  }

  for (std::size_t i = 0; i < rows; ++i)
  {
    for (std::size_t j = 0; j < cols; ++j)
    {
      double sum = 0.0;
      std::size_t count = 0;

      for (int di = -1; di <= 1; ++di)
      {
        for (int dj = -1; dj <= 1; ++dj)
        {
          if (di == 0 && dj == 0)
          {
            continue;
          }

          if ((di == -1 && i == 0)
              || (di == 1 && i == rows - 1)
              || (dj == -1 && j == 0)
              || (dj == 1 && j == cols - 1))
          {
            continue;
          }

          std::size_t ni = i + di;
          std::size_t nj = j + dj;
          sum += matrix[ni * cols + nj];
          ++count;
        }
      }
      smoothed[i * cols + j] = (count > 0) ? sum / count : matrix[i * cols + j];
    }
  }
}
