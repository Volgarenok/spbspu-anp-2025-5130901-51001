#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <memory>

namespace borisov
{
  size_t countLocalMaximum(const int * matrix, size_t rows, size_t cols)
  {
    if (rows < 3 || cols < 3) {
      return 0;
    }
    size_t count = 0;
    for (size_t i = 1; i < rows - 1; i++) {
      for (size_t j = 1; j < cols - 1; j++) {
        int current = matrix[i * cols + j];
        bool is_max = true;
        size_t start_i = i - 1;
        size_t start_j = j - 1;
        for (size_t di = 0; di < 3 && is_max; di++) {
          for (size_t dj = 0; dj < 3 && is_max; dj++) {
            if (di == 1 && dj == 1) {
              continue;
            }
            size_t nghbr_i = start_i + di;
            size_t nghbr_j = start_j + dj;
            size_t nghbr_idx = nghbr_i * cols + nghbr_j;
            if (current <= matrix[nghbr_idx]) {
              is_max = false;
            }
          }
        }
        if (is_max) {
          count++;
        }
      }
    }
    return count;
  }

  int getMaxSumAntidiagonal(const int * matrix, size_t rows, size_t cols)
  {
    size_t n = 0;
    if (rows < cols) {
      n = rows;
    } else {
      n = cols;
    }
    int max_diagsum = 0;
    for (size_t sum = 0; sum <= 2 * (n - 1); ++sum) {
      if (sum == n - 1) {
        continue;
      }
      int diagsum = 0;
      for (size_t i = 0; i < n; ++i) {
        if (sum >= i) {
          size_t j = sum - i;
          if (j < n) {
            diagsum += matrix[i * cols + j];
          }
        }
      }
      if (diagsum > max_diagsum) {
        max_diagsum = diagsum;
      }
    }
    return max_diagsum;
  }
}

int main(int argc, char ** argv)
{
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }

  char * endptr = nullptr;
  long mode = std::strtol(argv[1], std::addressof(endptr), 10);

  if (*endptr != '\0' || endptr == argv[1]) {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }

  if (mode != 1 && mode != 2) {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cerr << "open input file failed\n";
    return 2;
  }

  size_t rows = 0, cols = 0;
  if (!(input >> rows >> cols)) {
    std::cerr << "read matrix failed\n";
    return 2;
  }

  if (rows == 0 || cols == 0) {
    std::ofstream output(argv[3]);
    output << "0\n0\n";
    return 0;
  }

  const size_t MAX_STATIC_ELEMENTS = 10000;
  if (mode == 1 && rows > MAX_STATIC_ELEMENTS / cols) {
    std::cerr << "Error: matrix too large for static array\n";
    return 2;
  }

  int * matrix = nullptr;
  int fixed_matrix[10000] = {};

  if (mode == 1) {
    matrix = fixed_matrix;
  } else {
    try {
      matrix = new int[rows * cols];
    } catch (const std::bad_alloc& e) {
      std::cerr << "memory allocation failed\n";
      return 2;
    }
  }

  size_t total = rows * cols;
  for (size_t i = 0; i < total; ++i) {
    if (!(input >> matrix[i])) {
      std::cerr << "read matrix failed\n";
      if (mode == 2) {
        delete[] matrix;
      }
      return 2;
    }
  }

  std::ofstream output(argv[3]);
  if (!output.is_open()) {
    std::cerr << "open output file failed\n";
    if (mode == 2) {
      delete[] matrix;
    }
    return 2;
  }

  output << borisov::countLocalMaximum(matrix, rows, cols) << "\n";
  output << borisov::getMaxSumAntidiagonal(matrix, rows, cols) << "\n";

  if (mode == 2) {
    delete[] matrix;
  }

  return 0;
}
