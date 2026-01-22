#include <iostream>
#include <fstream>

namespace petrenko
{
  bool checkTriangular(const int *matrix, size_t row, size_t col)
  {
    if (row == 0 || col == 0)
    {
      return false;
    }
    size_t nice_row = 0;
    for (size_t i = 0; i < row; ++i)
    {
      size_t counter_zero = 0;
      for (size_t j = 0; j < col; ++j)
      {
        if (*matrix == 0)
        {
          ++counter_zero;
        }
        ++matrix;
      }
      if (counter_zero == row - (i + 1))
      {
        nice_row += 1;
      }
    }
    return (nice_row == row);
  }

  size_t checkIdentic(const int *matrix, size_t row, size_t col)
  {
    if (row == 0 || col == 0)
    {
      return 0;
    }
    size_t replay_col = 0;
    for (size_t i = 0; i < col; ++i)
    {
      int last = matrix[0 * col + i];
      for (size_t j = 1; j < row; ++j)
      {
        if (matrix[j * col + i] == last)
        {
          ++replay_col;
          last = matrix[j * col + i];
          break;
        }
        last = matrix[j * col + i];
      }
    }
    return col - replay_col;
  }

  size_t fillMatrix(std::ifstream &in, int *matrix, size_t row, size_t col)
  {
    size_t err = 0;
    for (size_t i = 0; i < row * col; ++i)
    {
      if (!(in >> *matrix))
      {
        ++err;
      }
      ++matrix;
    }
    return row * col - err;
  }

  void needRemove(const int *matrix, size_t parm)
  {
    if (parm == 2 && matrix != nullptr)
    {
      delete [] matrix;
    }
  }
}

int main(int argc, char **argv)
{
  if (argc < 4)
  {
    std::cerr << "Too few arguments" << '\n';
    return 1;
  }
  else if (argc > 4)
  {
    std::cerr << "Too many arguments" << '\n';
    return 1;
  }

  char *end = nullptr;
  long num = std::strtol(argv[1], std::addressof(end), 10);
  if (num != 1 && num != 2)
  {
    std::cerr << "First argument is out of range\n";
    return 1;
  }
  else if (std::isspace(*end) || end == argv[1] || *end != '\0')
  {
    std::cerr << "First argument is not a number\n";
    return 1;
  }

  int first_parm = num;
  char *name_output = argv[3];
  std::ifstream input(argv[2]);

  if (!input.is_open())
  {
    std::cerr << "Error open file" << '\n';
    return 2;
  }

  size_t row = 0, col = 0;
  input >> row >> col;
  if (input.fail())
  {
    std::cerr << "Error no matrix element" << '\n';
    return 2;
  }

  int *matrix = nullptr;
  int matrixa[10000];
  if (first_parm == 1)
  {
    matrix = matrixa;
  }
  else
  {
    matrix = new int[row * col];
  }

  if (petrenko::fillMatrix(input, matrix, row, col) != (row * col))
  {
    std::cerr << "Error no matrix element" << '\n';
    petrenko::needRemove(matrix, first_parm);
    return 2;
  }
  bool trian = petrenko::checkTriangular(matrix, row, col);
  size_t ident = petrenko::checkIdentic(matrix, row, col);
  std::ofstream(name_output) << std::boolalpha << trian << ' ' << ident << '\n';
  petrenko::needRemove(matrix, first_parm);
  return 0;
}
