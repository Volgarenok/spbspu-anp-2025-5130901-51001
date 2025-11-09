#include <iostream>
#include <fstream>
#include <cstring>

namespace hachaturyanov {
  void task5(int* matrix, int n, int m);
}

void hachaturyanov::task5(int* matrix, int rows, int cols) {
  int top = 0, left = 0, bottom = rows - 1, right = cols - 1;
  int d = 1;
  while(top <= bottom && left <= right) {
    for (int i = top * cols + left; i <= top * cols + right; i++) {
      matrix[i] -= d;
      d++;
    }

    for (int i = (top + 1) * cols + right; i <= (bottom + 1) * cols - left; i += cols) {
      matrix[i] -= d;
      d++;
    }

    for (int i = bottom * cols + right - 1; i >= bottom * cols + left; i--) {
      matrix[i] -= d;
      d++;
    }

    for (int i = (bottom - 1) * cols + left; i >= (top + 1) * cols + left; i -= cols) {
      matrix[i] -= d;
      d++;
    }

    top++;
    left++;
    bottom--;
    right--;
  }
}


int main(int argc, char** argv) {
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  } else if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }

  if (!strcmp(argv[1],"1") && !strcmp(argv[1],"2")) {
    std::cerr << "Incorrect first parameter\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cerr << "Cannot open the input file\n";
    return 2;
  }

  int rows = 0, cols = 0;
  input >> rows >> cols;
  if (!input.good()) {
    std::cerr << "Error while reading the file\n";
    return 2;
  }

  if (!strcmp(argv[1], "1")) {
    int matrix[rows * cols];
    for (size_t i = 0; i < rows * cols; i++) {
      input >> matrix[i];
      if (!input.good()) {
        std::cerr << "Error while reading the file\n";
        return 2;
      }
    }
    input.close();
    hachaturyanov::task5(matrix, rows, cols);
    std::ofstream output(argv[3]);
    if (!output.is_open()) {
      std::cerr << "Cannot open the output file\n";
      return 2;
    }
    output << rows << ' ' << cols;
    for (size_t i = 0; i < rows * cols; i++) {
      output << ' ' << matrix[i];
    }
    output << "\n";
    output.close();
  }

  return 0;
}
