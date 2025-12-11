#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <cstddef>

namespace karpenko
{
  const std::size_t MAX_DIMENSION = 100;
  const std::size_t MAX_SIZE = MAX_DIMENSION * MAX_DIMENSION;

  void transformMatrixSpiral(std::size_t rows, std::size_t cols, int matrix[])
  {
    if (rows == 0 || cols == 0)
      return;

    std::size_t top = 0, bottom = rows - 1;
    std::size_t left = 0, right = cols - 1;
    int counter = 1;

    while (top <= bottom && left <= right)
    {
      for (std::size_t i = top; i <= bottom; i++)
        matrix[i * cols + left] += counter++;
      left++;

      for (std::size_t i = left; i <= right; i++)
        matrix[bottom * cols + i] += counter++;
      bottom--;

      if (left <= right)
      {
        for (std::size_t i = bottom + 1; i > top; i--)
          matrix[(i - 1) * cols + right] += counter++;
        right--;
      }

      if (top <= bottom)
      {
        for (std::size_t i = right + 1; i > left; i--)
          matrix[top * cols + (i - 1)] += counter++;
        top++;
      }
    }
  }

  void createSmoothedMatrix(std::size_t rows, std::size_t cols, int matrix[], double smoothed[])
  {
    if (rows == 0 || cols == 0)
      return;

    for (std::size_t i = 0; i < rows; i++)
    {
      for (std::size_t j = 0; j < cols; j++)
      {
        double sum = 0.0;
        std::size_t count = 0;

        for (int di = -1; di <= 1; di++)
        {
          for (int dj = -1; dj <= 1; dj++)
          {
            if (di == 0 && dj == 0)
              continue;

            if ((di == -1 && i == 0) || (di == 1 && i == rows - 1) ||
                (dj == -1 && j == 0) || (dj == 1 && j == cols - 1))
              continue;

            std::size_t ni = i + di;
            std::size_t nj = j + dj;
            sum += matrix[ni * cols + nj];
            count++;
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

    if (rows > MAX_DIMENSION || cols > MAX_DIMENSION)
    {
      std::cerr << "Error: Matrix dimensions exceed maximum allowed size\n";
      return 0;
    }

    for (std::size_t i = 0; i < rows; i++)
    {
      for (std::size_t j = 0; j < cols; j++)
      {
        if (!(stream >> matrix[i * cols + j]))
        {
          std::cerr << "Error: Cannot read element at (" << i << ", " << j << ")\n";
          return 0;
        }
      }
    }
    return 1;
  }

  int writeMatrix(std::ostream &stream, const int matrix[], std::size_t rows, std::size_t cols)
  {
    stream << rows << " " << cols;
    for (std::size_t i = 0; i < rows; i++)
    {
      for (std::size_t j = 0; j < cols; j++)
      {
        stream << " " << matrix[i * cols + j];
      }
    }
    return stream.good() ? 1 : 0;
  }

  int writeMatrix(std::ostream &stream, const double matrix[], std::size_t rows, std::size_t cols)
  {
    stream << std::fixed << std::setprecision(1) << rows << " " << cols;
    for (std::size_t i = 0; i < rows; i++)
    {
      for (std::size_t j = 0; j < cols; j++)
      {
        stream << " " << matrix[i * cols + j];
      }
    }
    return stream.good() ? 1 : 0;
  }

  int checkIsNumber(const char *str)
  {
    if (!str || !*str)
    {
      std::cerr << "Error: First parameter is null or empty\n";
      return 0;
    }

    char *endptr = nullptr;
    std::strtol(str, &endptr, 10);

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

  const char *input_file = argv[2];
  const char *output_file = argv[3];

  int input_matrix[karpenko::MAX_SIZE];
  std::size_t rows, cols;

  std::ifstream input_stream(input_file);
  if (!input_stream)
  {
    std::cerr << "Error: Cannot open input file '" << input_file << "'\n";
    return 2;
  }
  if (!karpenko::readMatrix(input_stream, input_matrix, rows, cols))
  {
    return 2;
  }
  std::ofstream output_stream(output_file);
  if (!output_stream)
  {
    std::cerr << "Error: Cannot open output file '" << output_file << "'\n";
    return 2;
  }
  if (operation == 1)
  {
    if (rows > 0 && cols > 0)
    {
      karpenko::transformMatrixSpiral(rows, cols, input_matrix);
    }
    if (rows == 0 || cols == 0)
    {
      output_stream << rows << " " << cols;
    }
    else
    {
      if (!karpenko::writeMatrix(output_stream, input_matrix, rows, cols))
      {
        return 2;
      }
    }
  }
  else
  {
    if (rows == 0 || cols == 0)
    {
      output_stream << rows << " " << cols;
    }
    else
    {
      double output_matrix[karpenko::MAX_SIZE];
      karpenko::createSmoothedMatrix(rows, cols, input_matrix, output_matrix);
      if (!karpenko::writeMatrix(output_stream, output_matrix, rows, cols))
      {
        return 2;
      }
    }
  }

  std::cout << (operation == 1 ? "Spiral transformation" : "Matrix smoothing") << " completed successfully\n";

  return 0;
}
