#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>

namespace volkovich
{
  void decreaseSpiral(int *matrix, size_t rows, size_t columns);
  int minDiagonal(const int *matrix, size_t rows, size_t columns);
  const size_t MAX_MATRIX_SIZE = 10000;
}

void volkovich::decreaseSpiral(int *matrix, size_t rows, size_t columns)
{
  int top = 0, bottom = rows - 1, left = 0, right = columns - 1;
  size_t i = 0;
  int minus_value = 1;
  while (top <= bottom)
  {
    for (i = top * columns + left; i <= top * columns + right; i++)
    {
      matrix[i] -= minus_value;
      minus_value++;
    }
    if (top == bottom)
    {
      break;
    }
    for (i = (top + 1) * columns + right; i <= (bottom + 1) * columns - left; i += columns)
    {
      matrix[i] -= minus_value;
      minus_value++;
    }
    for (i = (bottom * columns) + right - 1; i >= left + bottom * columns; i--)
    {
      matrix[i] -= minus_value;
      minus_value++;
    }
    for (i = (bottom - 1) * columns + left; i >= (top + 1) * columns + left; i -= columns)
    {
      matrix[i] -= minus_value;
      minus_value++;
    }
    top++;
    bottom--;
    right--;
    left++;
  }
}

int volkovich::minDiagonal(const int *matrix, size_t rows, size_t columns)
{
  if (rows == 0 && columns == 0)
  {
    return 0;
  }
  size_t s = 0;
  int sum = 0;
  for (size_t i = 0; i < rows; i++)
  {
    if (s >= i && s - i < columns)
    {
      sum += matrix[i * columns + (s - i)];
    }
  }
  int res = sum;
  for (s = 1; s <= rows + columns - 2; s++)
  {
    int sum = 0;
    for (size_t i = 0; i < rows; i++)
    {
      if (s >= i && s - i < columns)
      {
        sum += matrix[i * columns + (s - i)];
      }
    }
    res = std::min(sum, res);
  }
  return res;
}

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments" << "\n";
    return 1;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments" << "\n";
    return 1;
  }
  if ((std::strcmp(argv[1], "1") && std::strcmp(argv[1], "2")))
  {
    std::cerr << "First argument out of range" << "\n";
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input)
  {
    std::cerr << "Cannot open file" << "\n";
    return 1;
  }

  size_t rows = 0, columns = 0;

  if (!(input >> rows >> columns))
  {
    return 1;
  }
  int *matrix = nullptr;
  int *dyn_mtx = nullptr;

  int mtx[volkovich::MAX_MATRIX_SIZE];
  if (!std::strcmp(argv[1], "1"))
  {
    matrix = mtx;
  }
  else
  {
    try
    {
      dyn_mtx = new int[rows * columns];
    }
    catch (const std::bad_alloc &)
    {
      std::cerr << "Bad alloc" << "\n";
      return 1;
    }
    matrix = dyn_mtx;
  }
  for (size_t i = 0; i < rows * columns; i++)
  {
    input >> matrix[i];
    if (!input.good())
    {
      std::cerr << "Error reading file" << "\n";
      delete[] dyn_mtx;
      return 1;
    }
  }
  std::ofstream output(argv[3]);
  if (!output)
  {
    delete[] dyn_mtx;
    std::cerr << "Bad file" << "\n";
    return 1;
  }
  volkovich::decreaseSpiral(matrix, rows, columns);
  int res = volkovich::minDiagonal(matrix, rows, columns);
  output << res << ' ';
  output << rows << ' ' << columns;
  for (size_t i = 0; i < rows * columns; i++)
  {
    output << ' ' << matrix[i];
  }
  delete[] dyn_mtx;
  output << "\n";
  return 0;
}
