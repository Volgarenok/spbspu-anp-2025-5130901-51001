#include <iostream>
#include "functions_for_tasks.hpp"

int main(int argc, char **argv)
{
  using namespace vishnyakov;

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
    catch (const std::bad_alloc &)
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
  int result = biggiestDiagonal(matrix, row, column);
  spiralReduction(matrix, row, column);
  output << row << ' ' << column;
  for (size_t i = 0; i < row * column; ++i)
  {
    output << ' ' << matrix[i];
  }
  output << '\n' << result << '\n';
  delete[] dynamic_matrix;
  return 0;
}
