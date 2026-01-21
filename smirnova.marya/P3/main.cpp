#include <iostream>
#include <fstream>
#include <cstdlib>

#include "task_variants.hpp"

int main(int argc, char** argv)
{
  if (argc != 4)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  char* arg = argv[1];
  if ((arg[0] != '1' && arg[0] != '2') || arg[1] != '\0')
  {
    std::cerr << "Garbage in argument 1\n";
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input.is_open())
  {
    std::cerr << "Cannot open input file\n";
    return 1;
  }
  const size_t BUF = 10000;
  size_t rows = 0, cols = 0;
  if (!(input >> rows >> cols) || rows * cols > BUF)
  {
    std::cerr << "Error: invalid matrix size\n";
    return 2;
  }
  if (rows == 0 || cols == 0)
  {
    std::ofstream output(argv[3]);
    if (!output.is_open()) {
      std::cerr << "Cannot open output file\n";
      return 1;
    }
    output << "0 0\n0\n";
    return 0;
  }
  int *matrix = nullptr;
  int fixedMatrix[BUF] = {};
  if (arg[0] == '2')
  {
    matrix = new int[rows * cols];
  }
  else
  {
    matrix = fixedMatrix;
  }
  for (size_t i = 0; i < rows * cols; ++i)
  {
    if (!(input >> matrix[i]))
    {
      std::cerr << "Error: invalid element or out of range\n";
      if (arg[0] == '2')
      {
        delete[] matrix;
      }
      return 1;
    }
  }
  char extra = '\0';
  if (input >> extra)
  {
    std::cerr << "Error: extra data in input file (e.g., letter '" << extra << "')\n";
    return 2;
  }
  input.close();
  std::ofstream output(argv[3]);
  if (!output.is_open()) {
    std::cerr << "Cannot open output file\n";
    if (arg[0] == '2')
    {
      delete[] matrix;
    }
    return 1;
  }
  output << smirnova::findLocalMaximum(rows, cols, matrix) << "\n";
  output << smirnova::findElementsDiagonal(rows, cols, matrix) << "\n";
  if (arg[0] == '2')
  {
    delete[] matrix;
  }

  return 0;
}
