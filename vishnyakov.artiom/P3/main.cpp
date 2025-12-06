#include <iostream>
#include <fstream>
namespace vishnyakov
{
  void spiral_reduction(int *matrix, size_t row, size_t column, std::ostream &output);
  int biggiest_diagonal(const int *matrix, size_t row, size_t column);
  size_t move(size_t type, size_t column, size_t start);
  size_t is_in(size_t *array, size_t value, size_t size);
}
size_t vishnyakov::move(size_t type, size_t column, size_t start)
{
  if(start == 0)
  {
    return 1;
  }
  switch (type%4)
  {
  case 0:
    return start-column;
    break;
  case 1:
    return start+1;
    break;
  case 2:
    return start+column;
    break;
  case 3:
    return start-1;
    break;
  }
  return start;
}
size_t vishnyakov::is_in(size_t *array, size_t value, size_t size)
{
  for (size_t i=0; i<size; ++i)
  {
    if (value==array[i])
    {
      return 1;
    }
  }
  return 0;
}
void vishnyakov::spiral_reduction(int *matrix, size_t row, size_t column, std::ostream &output)
{
  size_t start = row*column-column, type_of_mooving = 0;
  size_t *completed_values = new size_t[column*row];
  for (size_t i = 0; i<column*row; ++i)
  {
    completed_values[i]=column*row;
  }
  for (size_t i = 0; i<row*column; ++i)
  {
    if (vishnyakov::move(type_of_mooving,column,start)>row*column)
    {
      type_of_mooving+=1;
    }
    matrix[start] -= i+1;
    completed_values[i]=start;
    type_of_mooving += vishnyakov::is_in(completed_values, vishnyakov::move(type_of_mooving,column,start), row*column);
    start = vishnyakov::move(type_of_mooving,column,start);
  }
  output << row << ' ' << column;
  for (size_t i = 0; i<row*column;++i)
  {
    output << ' ' << matrix[i];
  }
  delete[] completed_values;
}
int vishnyakov::biggiest_diagonal(const int *matrix, size_t row, size_t column)
{
  if (row == 0 || column == 0)
  {
    return 0;
  }
  int sum = 0, max_sum = matrix[column-1];
  size_t k = 0;
  for (size_t i = 1; i<column-1; ++i)
  {
    sum = matrix[i];
    k = i+column+1;
    while (k%column!=0 && k<row*column)
    {
      sum += matrix[k];
      k += column+1;
    }
    if(sum>max_sum)
    {
      max_sum = sum;
    }
  }
  for (size_t j=column; j<row*column; j+=column)
  {
    sum = matrix[j];
    k = j+column+1;
    while (k%column!=0 && k<row*column)
    {
      sum += matrix[k];
      k += column+1;
    }
    if(sum>max_sum)
    {
      max_sum = sum;
    }
  }
  return max_sum;
}
int main(int argc, char ** argv)
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
  int num = std::atoi(argv[1]);
  if (num != 1 && num != 2)
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
    input.close();
    std::cerr << "Error reading matrix sizes\n";
    return 2;
  }
  int *matrix = nullptr;
  int *dynamic_matrix = nullptr;
  if (num == 1)
  {
    int fixed_len_matrix[10000];
    matrix = fixed_len_matrix;
  }
  else
  {
    try
    {
      dynamic_matrix = new int[row * column];
    }
    catch (...)
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
      input.close();
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
  int result = vishnyakov::biggiest_diagonal(matrix, row, column);
  vishnyakov::spiral_reduction(matrix, row, column, output);
  output << '\n' << result;
  output.close();
  delete[] dynamic_matrix;
  return 0;
}

