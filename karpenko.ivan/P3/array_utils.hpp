#ifndef ARRAY_UTILS_HPP
#define ARRAY_UTILS_HPP

#include <iostream>
#include <cstddef>

namespace karpenko
{
  const std::size_t kMaxDimension = 100;
  const std::size_t kMaxSize = kMaxDimension * kMaxDimension;

  std::size_t readMatrix(std::istream &stream, int matrix[], std::size_t rows, std::size_t cols);

  std::ostream &writeMatrix(std::ostream &stream, const int matrix[], std::size_t rows, std::size_t cols);

  std::ostream &writeMatrix(std::ostream &stream, const double matrix[], std::size_t rows, std::size_t cols);

  int checkIsNumber(const char *str);
}

#endif
