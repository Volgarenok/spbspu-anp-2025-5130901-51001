#include "array_utils.hpp"
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cstddef>

std::size_t karpenko::readMatrix(std::istream &stream, int matrix[], std::size_t rows, std::size_t cols)
{
  if (rows == 0 || cols == 0)
  {
    return 0;
  }

  std::size_t total = rows * cols;
  std::size_t readCount = 0;
  for (std::size_t k = 0; k < total; ++k)
  {
    if (!(stream >> matrix[k]))
    {
      break;
    }
    ++readCount;
  }
  return readCount;
}

std::ostream &karpenko::writeMatrix(std::ostream &stream, const int matrix[], std::size_t rows, std::size_t cols)
{
  stream << rows << " " << cols;
  for (std::size_t i = 0; i < rows; ++i)
  {
    for (std::size_t j = 0; j < cols; ++j)
    {
      stream << " " << matrix[i * cols + j];
    }
  }
  return stream;
}

std::ostream &karpenko::writeMatrix(std::ostream &stream, const double matrix[], std::size_t rows, std::size_t cols)
{
  stream << std::fixed << std::setprecision(1) << rows << " " << cols;
  for (std::size_t i = 0; i < rows; ++i)
  {
    for (std::size_t j = 0; j < cols; ++j)
    {
      stream << " " << matrix[i * cols + j];
    }
  }
  return stream;
}

int karpenko::checkIsNumber(const char *str)
{
  if (!str || !*str)
  {
    return 0;
  }

  char *endptr = nullptr;
  std::strtol(str, std::addressof(endptr), 10);
  return (*endptr == '\0') ? 1 : 0;
}
