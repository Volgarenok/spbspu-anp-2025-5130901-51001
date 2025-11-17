#include <iostream>
#include <fstream>
#include <string>

namespace petrenko
{
  int variant1(int *matrix, size_t row, size_t col, std::string name_output)
  {
    if (row == 0 || col == 0)
    {
      std::ofstream(name_output) << "false";
      return 0;
    }
    if (row > col)
    {
      row = col;
    }
    if (col > row)
    {
      col = row;
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
    if (nice_row == row)
    {
      std::ofstream(name_output) << "true";
    }
    else
    {
      std::ofstream(name_output) << "false";
    }
    return 0;
  }

  int variant2(int **matrix, size_t row, size_t col, std::string name_output)
  {
      for (size_t i = 0; i < row; ++i)
      {
        for (size_t j = 0; j < col; ++j)
        {
          std::cout << matrix[i][j] << std::endl;
        }
      }
    return 0;
  }

  int take_matrix(std::string name, std::string name_output, int num)
  {
    std::ifstream input(name);

    if (!input.is_open())
    {
      std::cerr << "Error open file" << std::endl;
      return 2;
    }

    size_t row, col;
    input >> row >> col;
    if (input.fail())
    {
      std::cerr << "Error no matrix element1" << std::endl;
      return 2;
    }
    if (num == 1)
    {
      int matrix[row][col];
      for (size_t i = 0; i < row; ++i)
      {
        for (size_t j = 0; j < col; ++j)
        {
          if (input.fail())
          {
            std::cerr << "Error no matrix element2" << std::endl;
            return 2;
          }
          input >> matrix[i][j];
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

      for (size_t i = 0; i < row; ++i)
      {
        for (size_t j = 0; j < col; ++j)
        {
          if (input.fail())
          {
            std::cerr << "Error no matrix element3" << std::endl;
            return 2;
          }
          input >> matrix[i][j];
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
    std::cerr << "Not enough arguments" << std::endl;
    return 1;
  }
  else if (argc < 5)
  {
    std::cerr << "Too few arguments" << std::endl;
    return 1;
  }
  else if (argc > 5)
  {
    std::cerr << "Too many arguments" << std::endl;
    return 1;
  }

  if (!isdigit(*argv[2]))
  {
    std::cerr << "First parameter is not a number" << std::endl;
    return 1;
  }
  int first_parm = atoi(argv[2]);
  if (first_parm > 2 || first_parm < 1)
  {
    std::cerr << "First parameter is out of range" << std::endl;
    return 1;
  }

  std::string name_output = argv[4];
  std::string name_input = argv[3];
  return petrenko::take_matrix(name_input, name_output, first_parm);
}
