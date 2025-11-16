#include <iostream>
#include <fstream>
#include <string>

namespace petrenko
{
  int take_file(std::string name, int num)
  {
    std::ifstream input(name);

    if (!input.is_open())
    {
      std::cout << "Error open file" << std::endl;
      return 2;
    }

    size_t row, col;
    input >> row >> col;
    std::cout << row << col << std::endl;
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
            std::cerr << "Error no matrix element" << std::endl;
            return 2;
          }
        }
      }
      //функция для задачи
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
          input >> matrix[i][j];
        }
      }
      // функция для задачи
      for (size_t i = 0; i < row; ++i)
      {
        delete [] matrix[i];
      }
      delete [] matrix;
    }
    input.close();
    return num;
  }
}

int main(int argc, char ** argv)
{
  if (argc == 0)
  {
    std::cout << "Not enough arguments" << std::endl;
    return 1;
  }
  else if (argc < 5)
  {
    std::cout << "Too few arguments" << std::endl;
    return 1;
  }
  else if (argc > 5)
  {
    std::cout << "Too many arguments" << std::endl;
    return 1;
  }

  if (!isdigit(*argv[2]))
  {
    std::cout << "First parameter is not a number" << std::endl;
    return 1;
  }
  int first_parm = atoi(argv[2]);
  if (first_parm > 2 || first_parm < 1)
  {
    std::cout << "First parameter is out of range" << std::endl;
    return 1;
  }

  std::string name_input = argv[3];
  petrenko::take_file(name_input, first_parm);
  return 0;
}
