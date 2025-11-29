#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>

namespace alekseev {
  const size_t MAXSIZE = 10000;

  bool readMatrix(std::ifstream& input, int* matrix, size_t rows, size_t cols)
  {
    for (size_t i = 0; i < rows * cols; ++i) {
      if (!(input >> matrix[i])) {
        return false;
      }
    }
    return true;
  }

  size_t countSaddlePoints(const int* matrix, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }

    size_t saddleCount = 0;

    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        int current = matrix[i * cols + j];
        bool isMinInRow = true;
        bool isMaxInCol = true;

        for (size_t k = 0; k < cols; ++k) {
          if (matrix[i * cols + k] < current) {
            isMinInRow = false;
            break;
          }
        }

        if (!isMinInRow) {
          continue;
        }

        for (size_t k = 0; k < rows; ++k) {
          if (matrix[k * cols + j] > current) {
            isMaxInCol = false;
            break;
          }
        }

        if (isMaxInCol) {
          ++saddleCount;
        }
      }
    }

    return saddleCount;
  }

  size_t findLongestSeriesColumn(const int* matrix, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }

    size_t maxColumn = 0;
    size_t maxLength = 0;

    for (size_t j = 0; j < cols; ++j) {
      size_t currentLength = 1;
      size_t columnMaxLength = 1;

      for (size_t i = 1; i < rows; ++i) {
        if (matrix[i * cols + j] == matrix[(i - 1) * cols + j]) {
          ++currentLength;
        } else {
          if (currentLength > columnMaxLength) {
            columnMaxLength = currentLength;
          }
          currentLength = 1;
        }
      }

      if (currentLength > columnMaxLength) {
        columnMaxLength = currentLength;
      }

      if (columnMaxLength > maxLength) {
        maxLength = columnMaxLength;
        maxColumn = j;
      }
    }

    return maxColumn + 1;
  }

  void writeResults(std::ofstream& output, size_t saddleCount, size_t seriesColumn)
  {
    output << saddleCount << "\n";
    output << seriesColumn << "\n";
  }

  bool isValidNumber(const char* str)
  {
    if (str == nullptr || *str == '\0') {
      return false;
    }
    
    for (size_t i = 0; str[i] != '\0'; ++i) {
      if (!std::isdigit(str[i])) {
        return false;
      }
    }
    return true;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 4) {
    std::cerr << (argc < 4 ? "Not enough arguments" : "Too many arguments") << "\n";
    return 1;
  }

  const char* numStr = argv[1];
  const char* inputFile = argv[2];
  const char* outputFile = argv[3];

  if (!alekseev::isValidNumber(numStr)) {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }

  int taskNum = std::atoi(numStr);
  if (taskNum != 1 && taskNum != 2) {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }

  std::ifstream input(inputFile);
  if (!input.is_open()) {
    std::cerr << "Cannot open input file\n";
    return 2;
  }

  size_t rows = 0;
  size_t cols = 0;
  if (!(input >> rows >> cols)) {
    std::cerr << "Cannot read matrix dimensions\n";
    return 2;
  }

  if (rows == 0 && cols == 0) {
    std::ofstream output(outputFile);
    if (!output.is_open()) {
      std::cerr << "Cannot open output file\n";
      return 2;
    }
    alekseev::writeResults(output, 0, 0);
    return 0;
  }

  if (rows * cols > alekseev::MAXSIZE && taskNum == 1) {
    std::cerr << "Matrix size exceeds fixed array capacity\n";
    return 2;
  }

  if (taskNum == 1) {
    int matrix[alekseev::MAXSIZE];
    
    bool readSuccess = alekseev::readMatrix(input, matrix, rows, cols);
    input.close();

    if (!readSuccess) {
      std::cerr << "Cannot read matrix elements\n";
      return 2;
    }

    size_t saddleCount = alekseev::countSaddlePoints(matrix, rows, cols);
    size_t seriesColumn = alekseev::findLongestSeriesColumn(matrix, rows, cols);

    std::ofstream output(outputFile);
    if (!output.is_open()) {
      std::cerr << "Cannot open output file\n";
      return 2;
    }

    alekseev::writeResults(output, saddleCount, seriesColumn);
    output.close();
  } else {
    int* matrix = new int[rows * cols];
    
    bool readSuccess = alekseev::readMatrix(input, matrix, rows, cols);
    input.close();

    if (!readSuccess) {
      std::cerr << "Cannot read matrix elements\n";
      delete[] matrix;
      return 2;
    }

    size_t saddleCount = alekseev::countSaddlePoints(matrix, rows, cols);
    size_t seriesColumn = alekseev::findLongestSeriesColumn(matrix, rows, cols);

    std::ofstream output(outputFile);
    if (!output.is_open()) {
      std::cerr << "Cannot open output file\n";
      delete[] matrix;
      return 2;
    }

    alekseev::writeResults(output, saddleCount, seriesColumn);
    output.close();
    
    delete[] matrix;
  }

  return 0;
}
