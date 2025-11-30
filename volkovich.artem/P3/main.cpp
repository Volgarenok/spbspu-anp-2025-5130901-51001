#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>

const size_t MAX_MATRIX_SIZE = 10000;

namespace volkovich
{
  void decreaseSpiral(int* matrix, size_t rows, size_t columns);
  int minDiagonal(const int *matrix, size_t rows, size_t columns);
}

void volkovich::decreaseSpiral(int* matrix, size_t rows, size_t columns)
{
  int top{}, bottom = rows - 1, left{}, right = columns - 1;
  size_t i{};
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
  int res = INT_MAX;
  for (size_t s = 0; s <= rows + columns - 2; s++)
  {
    int sum = 0;
    for (int i = 0; i < rows; i++)
    {
      size_t j = s - i;
      if (j >= 0 && j < columns)
      {
        sum += matrix[i * columns + j];
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
    std::cerr << "Not enough arguments";
    return 1;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments";
    return 1;
  }
  if ((strcmp(argv[1], "1") && strcmp(argv[1], "2")))
  {
    std::cerr << "First argument out of range";
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input.is_open())
  {
    std::cerr << "Cannot open file";
    return 1;
  }

  size_t rows{}, columns{};

  if (!(input >> rows >> columns))
  {
    return 1;
  }
  int* matrix = nullptr;
  int* dyn_mtx = nullptr;
  if (!std::strcmp(argv[1], "1"))
  {
    int mtx[MAX_MATRIX_SIZE];
    matrix = mtx;
  } else {
    try {
      dyn_mtx = new int[rows*columns];
    } catch (const std::bad_alloc&) {
      std::cerr << "Bad alloc";
      return 1;
    }
    matrix=dyn_mtx;
  }
  for (size_t i{}; i < rows * columns; i++)
  {
    input >> matrix[i];
    if (!input.good() && !input.eof())
    {
      std::cerr << "Error reading file";
      delete[] dyn_mtx;
      return 1;
    }
  }
  input.close();
  std::ofstream output(argv[3]);
  try {
    volkovich::decreaseSpiral(matrix, rows, columns);
    int res = volkovich::minDiagonal(matrix, rows, columns);
    output << res;

    for (size_t i = 0; i < rows * columns; i++)
    {
      output << ' ' << matrix[i];
    }
  } catch (...) {
    delete[] dyn_mtx;
    return 1;
  }
  output.close();
  delete[] dyn_mtx;
  std::cout<<"\n";
  return 0;
}
