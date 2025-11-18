#include <iostream>
#include <fstream>
#include <string>

namespace petrenko
{
  void variant1(int * matrix, size_t row, size_t col, std::string name_output)
  {
    if (row == 0 || col == 0)
    {
      std::ofstream(name_output) << "false";
      return;
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
      std::ofstream(name_output) << "true";
    } else {
      std::ofstream(name_output) << "false";
    }
    return;
  }

  void variant2(int ** matrix, size_t row, size_t col, std::string name_output)
  {
    size_t right_col = 0;
    for (size_t i = 0; i < col; ++i)
    {
      int last = matrix[0][i];
      for (size_t j = 1; j < row; ++j)
      {
        if (matrix[j][i] == last)
        {
          ++right_col;
          last = matrix[j][i];
          break;
        }
        last = matrix[j][i];
      }
    }
    std::ofstream(name_output) << right_col;
    return;
  }

  int take_matrix(std::string name, std::string name_output, int num)
  {
    std::ifstream input(name);

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
    if (num == 1)
    {
      int matrix[row][col];
      for (size_t i = 0; i < row; ++i)
      {
        for (size_t j = 0; j < col; ++j)
        {
          input >> matrix[i][j];
          if (input.fail())
          {
            std::cerr << "Error no matrix element" << '\n';
            return 2;
          }
        }
      }
      variant1(&matrix[0][0], row, col, name_output);
      return 0;
    }
    else if (num == 2)
    {
      int **matrix = new int *[row];
      for (size_t i = 0; i < row; ++i)
      {
        matrix[i] = new int[col];
      }
      size_t i = 0;
      for (; i < row; ++i)
      {
        for (size_t j = 0; j < col; ++j)
        {
          input >> matrix[i][j];
          if (input.fail())
          {
            for (size_t e = 0; e < row; ++e)
            {
              delete [] matrix[e];
            }
            delete [] matrix;
            std::cerr << "Error no matrix element" << '\n';
            return 2;
          }
        }
      }
      variant2(matrix, row, col, name_output);
      for (size_t i = 0; i < row; ++i)
      {
        delete [] matrix[i];
      }
      delete [] matrix;
    }
    input.close();
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

  if (!isdigit(*argv[1]))
  {
    std::cerr << "First parameter is not a number" << '\n';
    return 1;
  }
  int first_parm = atoi(argv[1]);
  if (first_parm > 2 || first_parm < 1)
  {
    std::cerr << "First parameter is out of range" << '\n';
    return 1;
  }

  std::string name_output = argv[3];
  std::string name_input = argv[2];
  return petrenko::take_matrix(name_input, name_output, first_parm);
}
