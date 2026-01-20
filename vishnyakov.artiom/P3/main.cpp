#include <iostream>
#include <fstream>
namespace vishnyakov
{
  void spiralReduction(int *matrix, size_t row, size_t column);
  int biggiestDiagonal(const int *matrix, size_t row, size_t column);
  size_t move(size_t type, size_t column, size_t start);
  bool isIn(const size_t *array, size_t value, size_t size);
}
size_t vishnyakov::move(size_t type, size_t column, size_t start)
{
  if (start == 0)
  {
    return 1;
  }
  switch (type % 4)
  {
  case 0:
    return start - column;
  case 1:
    return start + 1;
  case 2:
    return start + column;
  case 3:
    return start - 1;
  }
  return start;
}
bool vishnyakov::isIn(const size_t *array, size_t value, size_t size)
{
  for (size_t i = 0; i < size; ++i)
  {
    if (value == array[i])
    {
      return true;
    }
  }
  return false;
}
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
int main(int argc, char **argv)
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  else if (argc > 4)
  {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  const char *string_num = argv[1];
  size_t num = 0;
  if (string_num[0] == '1' && string_num[1] == 0)
  {
    num = 1;
  }
  else if (string_num[0] == '2' && string_num[1] == 0)
  {
    num = 2;
  }
  else
  {
    std::cerr << "First argument is out of range or not a number\n";
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input.is_open())
  {
    std::cerr << "Read file failed\n";
    return 2;
  }
  size_t row = 0, column = 0;
  if (!(input >> row >> column))
  {
    std::cerr << "Error reading matrix sizes\n";
    return 2;
  }
  int *matrix = nullptr;
  int *dynamic_matrix = nullptr;
  int fixed_len_matrix[10000];
  if (num == 1)
  {
    matrix = fixed_len_matrix;
  }
  else
  {
    try
    {
      dynamic_matrix = new int[row * column];
    }
    catch (const std::bad_alloc&)
    {
      std::cerr << "Error: Memory allocation failed.\n";
      return 2;
    }
    matrix = dynamic_matrix;
  }
  for (size_t i = 0; i < row * column; ++i)
  {
    if (!(input >> matrix[i]))
    {
      std::cerr << "Error reading matrix\n";
      delete[] dynamic_matrix;
      return 2;
    }
  }
  input.close();
  std::ofstream output(argv[3]);
  if (!output.is_open())
  {
    std::cerr << "Error opening output file\n";
    delete[] dynamic_matrix;
    return 2;
  }
  int result = vishnyakov::biggiestDiagonal(matrix, row, column);
  vishnyakov::spiralReduction(matrix, row, column);
  output << row << ' ' << column;
  for (size_t i = 0; i < row * column; ++i)
  {
    output << ' ' << matrix[i];
  }
  output << '\n' << result << '\n';
  delete[] dynamic_matrix;
  return 0;
}

