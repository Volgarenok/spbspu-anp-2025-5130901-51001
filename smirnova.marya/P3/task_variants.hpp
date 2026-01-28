#ifndef TASK_VARIANTS_HPP
#define TASK_VARIANTS_HPP

#include <cstddef>

namespace smirnova
{
  size_t findLocalMaximum(size_t rows, size_t cols, const int *matrix);
  int findElementsDiagonal(size_t rows, size_t cols, const int *matrix);
}

#endif
