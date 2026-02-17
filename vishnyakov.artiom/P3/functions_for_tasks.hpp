#ifndef FUNCTIONS_FOR_TASKS_HPP
#define FUNCTIONS_FOR_TASKS_HPP
#include <fstream>
#include "functions.hpp"

namespace vishnyakov
{
  void spiralReduction(int *matrix, size_t row, size_t column);
  int biggiestDiagonal(const int *matrix, size_t row, size_t column);
}

#endif
