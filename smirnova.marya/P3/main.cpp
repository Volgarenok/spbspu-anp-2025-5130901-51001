#include <iostream>
#include <fstream>
#include <cstdlib>
namespace smirnova
{
  int localmaximum(size_t rows, size_t cols, int *matrix);
  int elementsdiagonal(size_t rows, size_t cols, int *matrix);
}
int smirnova::localmaximum(size_t rows, size_t cols, int *matrix)
{
  int count = 0;
  int d[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
  for (size_t i = 1; i < rows - 1; ++i)
  {
    for (size_t j = 1; j < cols - 1; ++j)
    {
      int val = matrix[i*cols + j];
      bool flag = true;
      for (int k = 0; k < 8 && flag; ++k)
      {
        int ni = static_cast<int>(i) + d[k][0];
        int nj = static_cast<int>(j) + d[k][1];
        if (ni >= 0 && ni < static_cast<int>(rows) && nj >= 0 && nj < static_cast<int>(cols) && matrix[ni*cols + nj] >= val)
        {
          if (matrix[ni*cols + nj] >= val)
          {
            flag = false;
          }
        }
      }
      if (flag)
      {
        ++count;
      }
    }
  }
  return count;
}
int smirnova::elementsdiagonal(size_t rows, size_t cols, int *matrix)
{
  size_t n;
  if (rows < cols)
  {
    n = rows;
  }
  else
  {
    n = cols;
  }
  int maxsum = 0;
  for (size_t sum = 0; sum <= 2*(n-1); ++sum)
  {
    if (sum == n-1)
    {
      continue;
    }
    int diagsum = 0;
    for (size_t i = 0; i < n; ++i)
    {
      size_t j = sum - i;
      if (j < n)
      {
        diagsum += matrix[i * cols + j];
      }
    }
    if (diagsum > maxsum)
    {
      maxsum = diagsum;
    }
  }
  return maxsum;
}
int main(int argc, char** argv)
{
  char* a = argv[1];
  if (argc != 4)
  {
    return 1;
  }
  while (*a == ' ')
  {
    ++a;
  }
  char m = *a;
  if (*a != '1' && *a != '2')
  {
    std::cerr << "Garbage in argument 1\n";
    return 1;
  }
  ++a;
  while (*a == ' ')
  {
    ++a;
  }
  if (*a != '\0')
  {
    std::cerr << "Garbage in argument 1\n";
    return 1;
  }
  int rows, cols;
  std::ifstream input(argv[2]);
  if (!input.is_open())
  {
    std::cerr << "Cannot open input file\n";
    return 1;
  }
  if (!(input >> rows >> cols) || rows < 0 || cols < 0 || static_cast<size_t>(rows) * static_cast<size_t>(cols) > 1000)
  {
    std::cerr << "Error: invalid matrix size\n";
    return 2;
  }
  if (rows == 0 || cols == 0)
  {
    std::ofstream output(argv[3]);
    output << "0 0\n0\n";
    return 0;
  }
  size_t rows1 = static_cast<size_t>(rows);
  size_t cols1 = static_cast<size_t>(cols);
  if (m == '1')
  {
    int matrix[rows1 * cols1];
    for (size_t i = 0; i < rows1 * cols1; ++i)
    {
      if (!(input >> matrix[i]) || std::abs(matrix[i]) > 1000000)
      {
        std::cerr << "Error: invalid element or out of range\n";
        return 2;
      }
    }
    char extra;
    if (input >> extra)
    {
      std::cerr << "Error: extra data in input file (e.g., letter '" << extra << "')\n";
      return 2;
    }
    input.close();
    std::ofstream output(argv[3]);
    output << smirnova::localmaximum(rows1, cols1, matrix) << "\n";
    output << smirnova::elementsdiagonal(rows1, cols1, matrix) << "\n";
    output.close();
  }
  else
  {
    int* matrix2 = new int[rows1 * cols1];
    for (size_t i = 0; i < rows1 * cols1; ++i)
    {
      if (!(input >> matrix2[i]) || std::abs(matrix2[i]) > 1000000)
      {
        std::cerr << "Error: invalid element or out of range\n";
        delete[] matrix2;
        return 2;
      }
    }
    char extra;
    if (input >> extra)
    {
      std::cerr << "Error: extra data in input file (e.g., letter '" << extra << "')\n";
      delete[] matrix2;
      return 2;
    }
    input.close();
    std::ofstream output(argv[3]);
    output << smirnova::localmaximum(rows1, cols1, matrix2) << "\n";
    output << smirnova::elementsdiagonal(rows1, cols1, matrix2) << "\n";
    output.close();
    delete[] matrix2;
  }
  return 0;
}
