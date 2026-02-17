#include "functions_for_tasks.hpp"

void vishnyakov::spiralReduction(int *matrix, size_t row, size_t column)
{
  size_t start = row * column - column, type_of_mooving = 0;
  size_t *completed_values = new size_t[column * row];
  for (size_t i = 0; i < column * row; ++i)
  {
    completed_values[i] = column * row;
  }
  for (size_t i = 0; i < row * column; ++i)
  {
    if (vishnyakov::move(type_of_mooving, column, start) > row * column)
    {
      type_of_mooving++;
    }
    matrix[start] -= i + 1;
    completed_values[i] = start;
    type_of_mooving += vishnyakov::isIn(completed_values, vishnyakov::move(type_of_mooving, column, start), row * column);
    start = vishnyakov::move(type_of_mooving, column, start);
  }
  delete[] completed_values;
}

int vishnyakov::biggiestDiagonal(const int *matrix, size_t row, size_t column)
{
  if (row == 0 || column == 0)
  {
    return 0;
  }
  int sum = 0, max_sum = matrix[column - 1];
  size_t k = 0;
  for (size_t i = 1; i < column - 1; ++i)
  {
    sum = matrix[i];
    k = i + column + 1;
    while (k % column != 0 && k < row * column)
    {
      sum += matrix[k];
      k += column + 1;
    }
    if (sum > max_sum)
    {
      max_sum = sum;
    }
  }
  for (size_t j = column; j < row * column; j += column)
  {
    sum = matrix[j];
    k = j + column + 1;
    while (k % column != 0 && k < row * column)
    {
      sum += matrix[k];
      k += column + 1;
    }
    if (sum > max_sum)
    {
      max_sum = sum;
    }
  }
  return max_sum;
}

