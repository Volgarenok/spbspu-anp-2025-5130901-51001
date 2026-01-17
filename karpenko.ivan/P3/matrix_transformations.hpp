#ifndef MATRIX_TRANSFORMATIONS_HPP
#define MATRIX_TRANSFORMATIONS_HPP

#include <cstddef>

namespace karpenko
{
  void transformMatrixSpiral(std::size_t rows, std::size_t cols, int matrix[]);
  void createSmoothedMatrix(std::size_t rows, std::size_t cols, const int matrix[], double smoothed[]);
}

#endif
