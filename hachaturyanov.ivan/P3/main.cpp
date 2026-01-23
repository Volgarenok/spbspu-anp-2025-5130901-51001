#include <iostream>
#include <fstream>
#include <cstring>

namespace hachaturyanov {
  void lowerClock(int* matrix, size_t rows, size_t cols);
  int minSumDiag(const int* matrix, size_t rows, size_t cols);
}

void hachaturyanov::lowerClock(int* matrix, size_t rows, size_t cols)
{
  int top = 0, left = 0, bottom = rows - 1, right = cols - 1;
  int d = 1;
  while (top <= bottom && left <= right) {
    for (size_t i = top * cols + left; i <= top * cols + right; i++) {
      matrix[i] -= d;
      d++;
    }

    for (size_t i = (top + 1) * cols + right; i <= (bottom + 1) * cols - left; i += cols) {
      matrix[i] -= d;
      d++;
    }

    for (size_t i = bottom * cols + right - 1; i >= bottom * cols + left; i--) {
      matrix[i] -= d;
      d++;
    }

    for (size_t i = (bottom - 1) * cols + left; i >= (top + 1) * cols + left; i -= cols) {
      matrix[i] -= d;
      d++;
    }

    top++;
    left++;
    bottom--;
    right--;
  }
}

int hachaturyanov::minSumDiag(const int* matrix, size_t rows, size_t cols)
{
  if (rows == 0 && cols == 0) {
    return 0;
  }
  int res = matrix[0];
  for (size_t diag = 0; diag <= rows + cols - 2; diag++) {
    if (diag == 0 || diag == cols - 1) {
      continue;
    }
    int s = 0;
    for (size_t i = 0; i < rows; i++) {
      if (diag >= i) {
        size_t j = diag - i;
        if (j < cols) {
          s += matrix[i * cols + j];
        }
      }
    }
    res = std::min(res, s);
  }
  return res;
}


int main(int argc, char** argv)
{
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  } else if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }

  if (std::strcmp(argv[1], "1") != 0 && std::strcmp(argv[1], "2") != 0) {
    std::cerr << "Incorrect first parameter\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cerr << "Cannot open the input file\n";
    return 2;
  }

  size_t rows = 0, cols = 0;
  if (!(input >> rows >> cols)) {
    std::cerr << "Error while reading the file\n";
    return 2;
  }

  int* matrix = nullptr;
  int temppointer[10000];

  if (!std::strcmp(argv[1], "1")) {
    matrix = temppointer;
  } else if (!std::strcmp(argv[1], "2")) {
    matrix = new int[rows * cols];
  }

  for (size_t i = 0; i < rows * cols; i++) {
    input >> matrix[i];
    if (!input.good()) {
      std::cerr << "Error while reading the file\n";
      if (!std::strcmp(argv[1], "2")) {
        delete[] matrix;
      }
      return 2;
    }
  }

  int res = hachaturyanov::minSumDiag(matrix, rows, cols);
  hachaturyanov::lowerClock(matrix, rows, cols);
  std::ofstream output(argv[3]);
  if (!output.is_open()) {
    std::cerr << "Cannot open the output file\n";
    if (!std::strcmp(argv[1], "2")) {
      delete[] matrix;
    }
    return 2;
  }
  output << rows << ' ' << cols;
  for (size_t i = 0; i < rows * cols; i++) {
    output << ' ' << matrix[i];
  }
  output << '\n' << res << '\n';
  if (!std::strcmp(argv[1], "2")) {
    delete[] matrix;
  }
  return 0;
}
