#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <exception>

namespace loseva {
  bool is_local_maximum(size_t row, size_t col, size_t rows, size_t cols, const int* matrix) {
    int current = matrix[row * cols + col];
    bool is_greater = true;
    for (int i = -1; i <= 1; ++i) {
      for (int j = -1; j <= 1; ++j) {
        if (i == 0 && j == 0) continue;
        if (matrix[(row + i) * cols + (col + j)] >= current) {
          is_greater = false;
        }
      }
    }
    return is_greater;
  }
  int count_local_maximums(size_t rows, size_t cols, const int* matrix) {
    if (rows < 3 || cols < 3) return 0;
    int count = 0;
    for (size_t i = 1; i < rows - 1; ++i) {
      for (size_t j = 1; j < cols - 1; ++j) {
        if (is_local_maximum(i, j, rows, cols, matrix)) {
          count++;
        }
      }
    }
    return count;
  }
  int max_secondary_diagonal_sum(size_t rows, size_t cols, const int* matrix) {
    if (rows == 0 || cols == 0) return 0;
    int max_sum = 0;
    bool sum_initialized = false;
    int r = static_cast<int>(rows);
    int c = static_cast<int>(cols);
    for (int k = 0; k <= (r + c - 2); ++k) {
      int current_diag_sum = 0;
      bool has_elements = false;
      for (int i = 0; i < r; ++i) {
        int j = k - i;
        if (j >= 0 && j < c) {
          current_diag_sum += matrix[i * cols + j];
          has_elements = true;
        }
      }
      if (has_elements) {
        if (!sum_initialized || current_diag_sum > max_sum) {
          max_sum = current_diag_sum;
          sum_initialized = true;
        }
      }
    }
    return max_sum;
  }
  bool try_read_matrix(std::istream& in, int* matrix, size_t count) {
    for (size_t i = 0; i < count; ++i) {
      if (!(in >> matrix[i])) return false;
    }
    char extra;
    bool has_no_tail = !(in >> extra);
    return has_no_tail;
  }
}
int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Error: Wrong number of arguments\n";
    return 1;
  }
  long mode = 0;
  try {
    mode = std::stoi(argv[1]);
  } catch (const std::exception& e) {
    std::cerr << "Error: Mode must be a number\n";
    return 1;
  }
  bool is_mode_valid = (mode == 1 || mode == 2);
  if (!is_mode_valid) {
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
    std::cerr << "Error: Invalid file or empty\n";
    return 2;
  }
  if (r == 0 || c == 0) {
    std::ofstream output(argv[3]);
    if (!output) return 1;
    output << 0 << "\n";
    return 0;
  }
  size_t total = r * c;
  int res_task3 = 0, res_task13 = 0;
  if (mode == 1) {
    const size_t LIMIT = 10000;
    if (total > LIMIT) {
      std::cerr << "Error: Matrix too large for stack\n";
      return 2;
    }
    int stack_arr[LIMIT];
    if (!loseva::try_read_matrix(input, stack_arr, total)) return 2;
    res_task3 = loseva::count_local_maximums(r, c, stack_arr);
    res_task13 = loseva::max_secondary_diagonal_sum(r, c, stack_arr);
  } else {
    int* heap_arr = nullptr;
    try {
        heap_arr = new int[total];
    } catch (const std::bad_alloc& e) {
        std::cerr << "Error: Memory allocation failed\n";
        return 2;
    }
    if (!loseva::try_read_matrix(input, heap_arr, total)) {
      delete[] heap_arr;
      return 2;
    }
    res_task3 = loseva::count_local_maximums(r, c, heap_arr);
    res_task13 = loseva::max_secondary_diagonal_sum(r, c, heap_arr);
    delete[] heap_arr;
  }
  std::ofstream output(argv[3]);
  if (!output) {
    std::cerr << "Error: Cannot open output file\n";
    return 1;
  }
  output << res_task3 << " " << res_task13 << "\n";
  return 0;
}
