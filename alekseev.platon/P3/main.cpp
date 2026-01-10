#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cerrno>
#include <limits>
#include <new>

namespace alekseev {
  const size_t MAXSIZE = 10000;

  bool mulOverflow(size_t a, size_t b, size_t& out)
  {
    if (a == 0 || b == 0) {
      out = 0;
      return false;
    }
    if (a > (std::numeric_limits< size_t >::max() / b)) {
      return true;
    }
    out = a * b;
    return false;
  }

  size_t readMatrix(std::istream& input, int* matrix, size_t rows, size_t cols)
  {
    const size_t need = rows * cols;
    size_t read = 0;
    for (; read < need; ++read) {
      if (!(input >> matrix[read])) {
        break;
      }
    }
    return read;
  }

  size_t countSaddlePoints(const int* matrix, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }

    size_t saddleCount = 0;

    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        const int current = matrix[i * cols + j];
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

  bool parseTaskNum(const char* s, long& outNum)
  {
    if (!s || *s == '\0') {
      return false;
    }

    errno = 0;
    char* end = nullptr;
    const long val = std::strtol(s, std::addressof(end), 10);

    if (errno != 0 || end == s || *end != '\0') {
      return false;
    }

    outNum = val;
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

  long taskNum = 0;
  if (!alekseev::parseTaskNum(numStr, taskNum)) {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }

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

  if (rows == 0 || cols == 0) {
    std::cerr << "Cannot interpret file content as a matrix\n";
    return 2;
  }

  size_t total = 0;
  if (alekseev::mulOverflow(rows, cols, total)) {
    std::cerr << "Matrix size is too large\n";
    return 2;
  }

  if (taskNum == 1 && total > alekseev::MAXSIZE) {
    std::cerr << "Matrix size exceeds fixed array capacity\n";
    return 2;
  }

  int fixed[alekseev::MAXSIZE];
  int* dyn = nullptr;
  int* data = fixed;

  if (taskNum == 2) {
    try {
      dyn = new int[total];
    } catch (const std::bad_alloc&) {
      std::cerr << "Cannot allocate memory\n";
      return 2;
    }
    data = dyn;
  }

  const size_t read = alekseev::readMatrix(input, data, rows, cols);
  if (read != total) {
    std::cerr << "Cannot read matrix elements\n";
    delete[] dyn;
    return 2;
  }

  const size_t saddleCount = alekseev::countSaddlePoints(data, rows, cols);
  const size_t seriesColumn = alekseev::findLongestSeriesColumn(data, rows, cols);

  std::ofstream output(outputFile);
  if (!output.is_open()) {
    std::cerr << "Cannot open output file\n";
    delete[] dyn;
    return 2;
  }

  alekseev::writeResults(output, saddleCount, seriesColumn);

  delete[] dyn;
  return 0;
}
