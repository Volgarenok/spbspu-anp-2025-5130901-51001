#include <iostream>
#include <fstream>
#include <cstdlib>
#include <memory>
#include "array_utils.hpp"
#include "matrix_transformations.hpp"

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    std::cerr << "Usage: " << (argc > 0 ? argv[0] : "program") << " num input output\n";
    std::cerr << "  num: 1 for array of fixed size, 2 for dynamic array\n";
    return 1;
  }

  if (!karpenko::checkIsNumber(argv[1]))
  {
    std::cerr << "Error: First parameter is not a valid number\n";
    return 1;
  }

  int num = std::atoi(argv[1]);
  if (num != 1 && num != 2)
  {
    std::cerr << "Error: First parameter must be 1 or 2\n";
    return 1;
  }

  const char *inputFile = argv[2];
  const char *outputFile = argv[3];

  std::size_t rows = 0;
  std::size_t cols = 0;

  std::ifstream inputStream(inputFile);
  if (!inputStream)
  {
    std::cerr << "Error: Cannot open input file '" << inputFile << "'\n";
    return 2;
  }

  std::ofstream outputStream(outputFile);
  if (!outputStream)
  {
    std::cerr << "Error: Cannot open output file '" << outputFile << "'\n";
    return 2;
  }

  if (!(inputStream >> rows >> cols))
  {
    std::cerr << "Error: Invalid matrix dimensions\n";
    return 2;
  }

  if (rows > karpenko::kMaxDimension || cols > karpenko::kMaxDimension)
  {
    std::cerr << "Error: Matrix dimensions exceed maximum allowed size\n";
    return 2;
  }

  if (rows == 0 && cols == 0)
  {
    outputStream << "0 0";
    std::cout << "Both matrix operations completed successfully (empty matrix)\n";
    return 0;
  }

  std::size_t totalElements = rows * cols;

  if (totalElements > karpenko::kMaxSize)
  {
    std::cerr << "Error: Matrix size exceeds maximum allowed size\n";
    return 2;
  }

  int fixedSizeMatrix[karpenko::kMaxSize] = {0};
  int *inputMatrix = fixedSizeMatrix;

  if (num == 2)
  {
    try
    {
      inputMatrix = new int[totalElements];
    }
    catch (const std::bad_alloc&)
    {
      std::cerr << "Error: Memory allocation failed for dynamic array\n";
      return 2;
    }
  }

  std::size_t readCount = karpenko::readMatrix(inputStream, inputMatrix, rows, cols);

  if (readCount != totalElements)
  {
    std::cerr << "Error: Not enough data\n";
    if (num == 2)
    {
      delete[] inputMatrix;
    }
    return 2;
  }

  int *spiralMatrix = nullptr;
  double *smoothedMatrix = nullptr;

  try
  {
    spiralMatrix = new int[totalElements];
    smoothedMatrix = new double[totalElements];
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Error: Memory allocation failed for processing\n";
    if (num == 2)
    {
      delete[] inputMatrix;
    }
    delete[] spiralMatrix;
    delete[] smoothedMatrix;
    return 2;
  }

  for (std::size_t i = 0; i < totalElements; ++i)
  {
    spiralMatrix[i] = inputMatrix[i];
  }

  karpenko::transformMatrixSpiral(rows, cols, spiralMatrix);
  karpenko::createSmoothedMatrix(rows, cols, inputMatrix, smoothedMatrix);

  karpenko::writeMatrix(outputStream, spiralMatrix, rows, cols);
  outputStream << "\n";
  karpenko::writeMatrix(outputStream, smoothedMatrix, rows, cols);

  if (!outputStream)
  {
    std::cerr << "Error: Failed to write matrix to output file\n";
    if (num == 2)
    {
      delete[] inputMatrix;
    }
    delete[] spiralMatrix;
    delete[] smoothedMatrix;
    return 2;
  }

  if (num == 2)
  {
    delete[] inputMatrix;
  }
  delete[] spiralMatrix;
  delete[] smoothedMatrix;

  std::cout << "Both matrix operations completed successfully\n";

  return 0;
}
