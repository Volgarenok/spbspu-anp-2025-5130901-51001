#include <iostream>
#include <fstream>
namespace vishnyakov
{
  void LFT_BOT_CLK(int *matrix, size_t row, size_t column, std::ostream &output);
  int MAX_SUM_SDG(const int *matrix, size_t row, size_t column);
}
int max_of(int n1, int n2)
{
  if (n1>n2)
  {
    return n1;
  }
  return n2;
}
size_t move(size_t type, size_t row, size_t column, size_t start)
{
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
}
size_t is_in(size_t *array,size_t value, size_t size)
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
void vishnyakov::LFT_BOT_CLK(int *matrix, size_t row, size_t column, std::ostream &output)
{
  size_t start = row*column-column, type_of_mooving = 0;
  size_t completed_values[column*row];
  for (size_t i = 0; i<column*row; ++i)
  {
    completed_values[i]=column*row;
  }
    for (size_t i = 0; i<row*column; ++i)
  {
    if (move(type_of_mooving,row,column,start)<0||move(type_of_mooving,row,column,start)>row*column)
    {
      type_of_mooving+=1;
    }
    matrix[start] -= i+1;
    completed_values[i]=start;
    type_of_mooving += is_in(completed_values, move(type_of_mooving,row,column,start), row*column);
    start = move(type_of_mooving,row,column,start);
  }
  output << row << ' ' << column;
  for (int i = 0; i<row*column;++i)
  {
    output << ' ' << matrix[i];
  }
}
int vishnyakov::MAX_SUM_SDG(const int *matrix, size_t row, size_t column)
{
  int sum = 0, max_sum = 0;
  size_t k = 0;
  for (size_t i = 1; i<column; ++i)
  {
    sum = matrix[i];
    k = i+column+1;
    while (k%column!=0 && k<row*column)
    {
      sum += matrix[k];
      k += column+1;
    }
    max_sum = max_of(sum,max_sum);
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
    max_sum = max_of(sum,max_sum);
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
  if ((*argv[1] != '1') && (*argv[1] != '2'))
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
  input >> row >> column;
  if (!input.good())
  {
    input.close();
    std::cerr << "Error reading matrix sizes\n";
    return 2;
  }
  int *matrix = nullptr;
  if ((*argv[1] == '1'))
  {
    int static_matrix[10000]= {};
    matrix = static_matrix;
  }
  else
  {
    try
    {
      matrix = new int[row * column];
    }
    catch (const std::bad_alloc&)
    {
      std::cerr << "Error allocating memory\n";
      return 2;
    }
  }
  for (size_t i = 0; i < row * column; ++i)
  {
    if (!(input >> matrix[i]))
    {
      input.close();
      std::cerr << "Error reading matrix\n";
      return 2;
    }
  }
  input.close();
  std::ofstream output(argv[3]);
  if (!output.is_open())
  {
    std::cerr << "Error opening output file\n";
    if ((*argv[1] == '2'))
    {
      delete[] matrix;
    }
    return 2;
  }
  int result = vishnyakov::MAX_SUM_SDG(matrix, row, column);
  vishnyakov::LFT_BOT_CLK(matrix,row,column, output);
  output << '\n' << result;
  output.close();
  if ((*argv[1] == '2'))
  {
    delete[] matrix;
  }
  return 0;
}