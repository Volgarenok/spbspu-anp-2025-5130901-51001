#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdexcept>

namespace loseva {
  bool isLocalMaximum(size_t row, size_t col, size_t cols, const int* matrix) {
    int current = matrix[row * cols + col];
    for (int i = -1; i <= 1; ++i) {
      for (int j = -1; j <= 1; ++j) {
        if (i == 0 && j == 0) {
          continue;
        }
        if (matrix[(row + i) * cols + (col + j)] >= current) {
          return false;
        }
      }
    }
    return true;
  }

  int countLocalMaximums(size_t rows, size_t cols, const int* matrix) {
    if (rows < 3 || cols < 3) {
      return 0;
    }
    int count = 0;
    for (size_t i = 1; i < rows - 1; ++i) {
      for (size_t j = 1; j < cols - 1; ++j) {
        if (isLocalMaximum(i, j, cols, matrix)) {
          count++;
        }
      }
    }
    return count;
  }

  int maxSecondaryDiagonalSum(size_t rows, size_t cols, const int* matrix) {
    if (rows == 0 || cols == 0) {
      return 0;
    }
    int maxSum = 0;
    bool sumInitialized = false;

    for (size_t k = 0; k <= (rows + cols - 2); ++k) {
      int currentDiagSum = 0;
      bool hasElements = false;
      for (size_t i = 0; i < rows; ++i) {
        if (k >= i) {
          size_t j = k - i;
          if (j < cols) {
            currentDiagSum += matrix[i * cols + j];
            hasElements = true;
          }
        }
      }
      if (hasElements) {
        if (!sumInitialized || currentDiagSum > maxSum) {
          maxSum = currentDiagSum;
          sumInitialized = true;
        }
      }
    }
    return maxSum;
  }

  bool tryReadMatrix(std::istream& in, int* matrix, size_t count) {
    for (size_t i = 0; i < count; ++i) {
      if (!(in >> matrix[i])) {
        return false;
      }
    }
    char extra;
    return !(in >> extra);
  }
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Error: Wrong number of arguments\n";
    return 1;
  }

  long mode = 0;
  try {
    mode = std::stol(argv[1]);
  } catch (...) {
    std::cerr << "Error: Mode must be a number\n";
    return 1;
  }

  if (mode != 1 && mode != 2) {
    std::cerr << "Error: First parameter must be 1 or 2\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input) {
    std::cerr << "Error: Cannot open input file\n";
    return 2;
  }

  size_t r = 0, c = 0;
  if (!(input >> r >> c)) {
    std::cerr << "Error: Invalid file format\n";
    return 2;
  }

  if (r == 0 || c == 0) {
    std::ofstream output(argv[3]);
    if (output) {
      output << "0 0\n";
    }
    return 0;
  }

  size_t total = r * c;
  int resTask3 = 0, resTask13 = 0;

  if (mode == 1) {
    const size_t LIMIT = 10000;
    if (total > LIMIT) {
      std::cerr << "Error: Matrix too large for stack\n";
      return 2;
    }
    int stackArr[LIMIT];
    if (!loseva::tryReadMatrix(input, stackArr, total)) {
      return 2;
    }
    resTask3 = loseva::countLocalMaximums(r, c, stackArr);
    resTask13 = loseva::maxSecondaryDiagonalSum(r, c, stackArr);
  } else {
    int* heapArr = nullptr;
    try {
      heapArr = new int[total];
    } catch (const std::bad_alloc&) {
      std::cerr << "Error: Memory allocation failed\n";
      return 2;
    }

    if (!loseva::tryReadMatrix(input, heapArr, total)) {
      delete[] heapArr;
      return 2;
    }
    resTask3 = loseva::countLocalMaximums(r, c, heapArr);
    resTask13 = loseva::maxSecondaryDiagonalSum(r, c, heapArr);
    delete[] heapArr;
  }

  std::ofstream output(argv[3]);
  if (!output) {
    std::cerr << "Error: Cannot open output file\n";
    return 1;
  }
  output << resTask3 << " " << resTask13 << "\n";

  return 0;
}
