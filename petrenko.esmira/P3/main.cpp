#include <iostream>
#include <fstream>

namespace petrenko
{
  bool chek_is_lower_triangular_matrix(int *matrix, size_t row, size_t col)
  {
    if (row == 0 || col == 0)
    {
      return 0;
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
    if (nice_row == row) {
      return true;
    } else {
      return false;
    }
  }

  size_t chek_how_col_identic_elem(int ** matrix, size_t row, size_t col)
  {
    size_t replay_col = 0;
    for (size_t i = 0; i < col; ++i)
    {
      int last = matrix[0][i];
      for (size_t j = 1; j < row; ++j)
      {
        if (matrix[j][i] == last)
        {
          ++replay_col;
          last = matrix[j][i];
          break;
        }
        last = matrix[j][i];
      }
    }
    return col - replay_col;
  }

  int fill_fix_array(std::ifstream &in, int * matrix, size_t row, size_t col)
  {
    for (size_t i = 0; i < row * col; ++i)
    {
      if (!(in >> *matrix))
      {
        std::cerr << "Error no matrix element" << '\n';
        return 2;
      }
      ++matrix;
    }
    return 0;
  }

  void remove_matrix(int ** matrix)
  {
    delete [] matrix[0];
    delete [] matrix;
  }

  int fill_dynamic_array(std::ifstream &in, int **matrix, size_t row, size_t col)
  {
    size_t i = 0;
    for (; i < row; ++i)
    {
      for (size_t j = 0; j < col; ++j)
      {
        if (!(in >> matrix[i][j]))
        {
          remove_matrix(matrix);
          std::cerr << "Error no matrix element" << '\n';
          return 2;
        }
      }
    }
    return 0;
  }
}

int main(int argc, char ** argv)
{
  if (argc == 0)
  {
    std::cerr << "Not enough arguments" << '\n';
    return 1;
  }
  else if (argc < 4)
  {
    std::cerr << "Too few arguments" << '\n';
    return 1;
  }
  else if (argc > 4)
  {
    std::cerr << "Too many arguments" << '\n';
    return 1;
  }

  char * end = nullptr;
  long num = std::strtol(argv[1], &end, 10);
  if (num != 1 && num != 2) {
    std::cerr << "First argument is out of range\n";
    return 1;
  } else if (isspace(*end) || end == argv[1] || *end != '\0') {
    std::cerr << "First argument is not a number\n";
    return 1;
  }

  int first_parm = std::atoi(argv[1]);
  char* name_output = argv[3];
  std::ifstream input(argv[2]);

  if (!input.is_open())
  {
    std::cerr << "Error open file" << '\n';
    return 2;
  }

  size_t row, col;
  input >> row >> col;
  if (input.fail())
  {
    std::cerr << "Error no matrix element" << '\n';
    return 2;
  }

  if (first_parm == 1)
  {
    int matrix[10000];
    if (petrenko::fill_fix_array(input, matrix, row, col) == 2)
    {
      return 2;
    }
    std::ofstream(name_output) << std::boolalpha << petrenko::chek_is_lower_triangular_matrix(matrix, row, col);
  } else if (first_parm == 2) {
    if (row == 0 || col == 0)
    {
      std::ofstream(name_output) << 0;
      return 0;
    }
    int **matrix = new int *[row];
    matrix[0] = new int[row * col];
    for (size_t i = 1; i != row; ++i)
    {
      matrix[i] = matrix[i - 1] + col;
    }
    if (petrenko::fill_dynamic_array(input, matrix, row, col) == 2)
    {
      return 2;
    }
    std::ofstream(name_output) << petrenko::chek_how_col_identic_elem(matrix, row, col);
    petrenko::remove_matrix(matrix);
  }
  return 0;
}
