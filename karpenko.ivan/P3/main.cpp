#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <cstddef>
#include <memory>

namespace karpenko
{
  const std::size_t kMaxDimension = 100;
  const std::size_t kMaxSize = kMaxDimension * kMaxDimension;

  void transformMatrixSpiral(std::size_t rows, std::size_t cols, int matrix[])
  {
    if (rows == 0 || cols == 0)
    {
      return;
    }

    std::size_t top = 0;
    std::size_t bottom = rows - 1;
    std::size_t left = 0;
    std::size_t right = cols - 1;
    int counter = 1;

    while (top <= bottom && left <= right)
    {
      for (std::size_t i = top; i <= bottom; ++i)
      {
        matrix[i * cols + left] += counter++;
      }
      ++left;

      for (std::size_t i = left; i <= right; ++i)
      {
        matrix[bottom * cols + i] += counter++;
      }
      --bottom;

      if (left <= right)
      {
        for (std::size_t i = bottom + 1; i > top; --i)
        {
          matrix[(i - 1) * cols + right] += counter++;
        }
        --right;
      }

      if (top <= bottom)
      {
        for (std::size_t i = right + 1; i > left; --i)
        {
          matrix[top * cols + (i - 1)] += counter++;
        }
        ++top;
      }
    }
  }

  void createSmoothedMatrix(std::size_t rows, std::size_t cols, const int matrix[], double smoothed[])
  {
    if (rows == 0 || cols == 0)
    {
      return;
    }

    for (std::size_t i = 0; i < rows; ++i)
    {
      for (std::size_t j = 0; j < cols; ++j)
      {
        double sum = 0.0;
        std::size_t count = 0;

        for (int di = -1; di <= 1; ++di)
        {
          for (int dj = -1; dj <= 1; ++dj)
          {
            if (di == 0 && dj == 0)
            {
              continue;
            }

            if ((di == -1 && i == 0) ||
                (di == 1 && i == rows - 1) ||
                (dj == -1 && j == 0) ||
                (dj == 1 && j == cols - 1))
            {
              continue;
            }

            std::size_t ni = i + di;
            std::size_t nj = j + dj;
            sum += matrix[ni * cols + nj];
            ++count;
          }
        }
        smoothed[i * cols + j] = (count > 0) ? sum / count : matrix[i * cols + j];
      }
    }
  }

  int readMatrix(std::istream &stream, int matrix[], std::size_t &rows, std::size_t &cols)
  {
    if (!(stream >> rows >> cols))
    {
      std::cerr << "Error: Invalid matrix dimensions\n";
      return 0;
    }

    if (rows > kMaxDimension || cols > kMaxDimension)
    {
      std::cerr << "Error: Matrix dimensions exceed maximum allowed size\n";
      return 0;
    }

    std::size_t expectedElements = rows * cols;
    std::size_t count = 0;

    for (std::size_t i = 0; i < rows; ++i)
    {
      for (std::size_t j = 0; j < cols; ++j)
      {
        if (!(stream >> matrix[i * cols + j]))
        {
          std::cerr << "Error: Cannot read element at (" << i << ", " << j << ")\n";
          return 0;
        }
        ++count;
      }
    }

    if (rows == 0 && cols == 0)
    {
      return 1;
    }

    return (count == expectedElements) ? 1 : 0;
  }

  std::ostream &writeMatrix(std::ostream &stream, const int matrix[], std::size_t rows, std::size_t cols)
  {
    stream << rows << " " << cols;
    for (std::size_t i = 0; i < rows; ++i)
    {
      for (std::size_t j = 0; j < cols; ++j)
      {
        stream << " " << matrix[i * cols + j];
      }
    }
    return stream;
  }

  std::ostream &writeMatrix(std::ostream &stream, const double matrix[], std::size_t rows, std::size_t cols)
  {
    stream << std::fixed << std::setprecision(1) << rows << " " << cols;
    for (std::size_t i = 0; i < rows; ++i)
    {
      for (std::size_t j = 0; j < cols; ++j)
      {
        stream << " " << matrix[i * cols + j];
      }
    }
    return stream;
  }

  int checkIsNumber(const char *str)
  {
    if (!str || !*str)
    {
      std::cerr << "Error: First parameter is null or empty\n";
      return 0;
    }

    char *endptr = nullptr;
    std::strtol(str, std::addressof(endptr), 10);

    if (*endptr != '\0')
    {
      std::cerr << "Error: First parameter is not a valid number\n";
      return 0;
    }
    return 1;
  }
}

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " num input output\n";
    std::cerr << "  num: 1 for spiral transformation, 2 for matrix smoothing\n";
    return 1;
  }

  if (!karpenko::checkIsNumber(argv[1]))
  {
    return 1;
  }

  int operation = std::atoi(argv[1]);
  if (operation != 1 && operation != 2)
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

  int *inputMatrix = nullptr;

  if (operation == 1)
  {
    static int fixedMatrix[karpenko::kMaxSize];
    inputMatrix = fixedMatrix;

    if (!karpenko::readMatrix(inputStream, inputMatrix, rows, cols))
    {
      return 2;
    }
  }
  else
  {
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

    if (rows == 0 || cols == 0)
    {
      outputStream << rows << " " << cols;
      std::cout << "Matrix smoothing completed successfully (empty matrix)\n";
      return 0;
    }

    std::size_t matrixSize = rows * cols;
    inputMatrix = new int[matrixSize];

    for (std::size_t i = 0; i < rows; ++i)
    {
      for (std::size_t j = 0; j < cols; ++j)
      {
        if (!(inputStream >> inputMatrix[i * cols + j]))
        {
          std::cerr << "Error: Cannot read element at (" << i << ", " << j << ")\n";
          delete[] inputMatrix;
          return 2;
        }
      }
    }
  }

  if (rows == 0 || cols == 0)
  {
    outputStream << rows << " " << cols;
    std::cout << (operation == 1 ? "Spiral transformation" : "Matrix smoothing") << " completed successfully (empty matrix)\n";
    if (operation == 2)
    {
      delete[] inputMatrix;
    }
    return 0;
  }
  if (operation == 1)
  {
    karpenko::transformMatrixSpiral(rows, cols, inputMatrix);
    karpenko::writeMatrix(outputStream, inputMatrix, rows, cols);
  }
  else
  {
    double *smoothedMatrix = new double[rows * cols];

    karpenko::createSmoothedMatrix(rows, cols, inputMatrix, smoothedMatrix);
    karpenko::writeMatrix(outputStream, smoothedMatrix, rows, cols);

    delete[] smoothedMatrix;
  }

  if (!outputStream)
  {
    std::cerr << "Error: Failed to write matrix to output file\n";

    if (operation == 2)
    {
      delete[] inputMatrix;
    }
    return 2;
  }

  if (operation == 2)
  {
    delete[] inputMatrix;
  }

  std::cout << (operation == 1 ? "Spiral transformation" : "Matrix smoothing") << " completed successfully\n";
  return 0;
}
