#include <iostream>
#include <fstream>

namespace loseva
{
  int max_secondary_diagonal_sum(size_t rows, size_t cols, const int* matrix);
  int count_local_maximums(size_t rows, size_t cols, const int* matrix);
}

int loseva::max_secondary_diagonal_sum(size_t rows, size_t cols, const int* matrix)
{
  if (rows == 0 || cols == 0) return 0;
  int max_sum = 0;
  int n = (rows < cols ? rows : cols);
  for (int diag_index = 0; diag_index <= 2 * (n - 1); ++diag_index) {
    if (diag_index == n - 1) continue;
    int diag_sum = 0;
    for (int i = 0; i < n; ++i) {
      int j = diag_index - i;
      if ((j >= 0) && (j < n)) {
        diag_sum += matrix[i * cols + j];
      }
    }
    if (diag_sum > max_sum) {
      max_sum = diag_sum;
    }
  }
  return max_sum;
}

int loseva::count_local_maximums(size_t rows, size_t cols, const int* matrix)
{
  if (rows < 3 || cols < 3) return 0;
  int count = 0;
  for (size_t i = 1; i < rows - 1; i++) {
    for (size_t j = 1; j < cols - 1; j++) {
      int current = matrix[i * cols + j];
      if (matrix[i * cols + j - 1] < current &&
          matrix[i * cols + j + 1] < current &&
          matrix[(i - 1) * cols + j] < current &&
          matrix[(i + 1) * cols + j] < current &&
          matrix[(i - 1) * cols + j - 1] < current &&
          matrix[(i + 1) * cols + j + 1] < current &&
          matrix[(i + 1) * cols + j - 1] < current &&
          matrix[(i - 1) * cols + j + 1] < current) {
        count++;
      }
    }
  }
  return count;
}
void process_matrix(int function_id, const char* input_file, const char* output_file)
{
  std::ifstream input(input_file);
  if (!input.is_open()) {
    std::cerr << "Cannot open input file\n";
    throw std::runtime_error("Cannot open input file");
  }
  size_t rows, cols;
  input >> rows >> cols;
  if (input.fail()) {
    std::cerr << "Cannot read matrix dimensions\n";
    throw std::runtime_error("Cannot read matrix dimensions");
  }
  if (rows == 0 || cols == 0) {
    std::ofstream output(output_file);
    output << "0";
    return;
  }
  int* matrix = new int[rows * cols];
  for (size_t i = 0; i < rows * cols; i++) {
    input >> matrix[i];
    if (input.fail()) {
      std::cerr << "Cannot read matrix element\n";
      delete[] matrix;
      throw std::runtime_error("Cannot read matrix element");
    }
  }
  
  int result = 0;
  
  if (function_id == 1) {
    result = loseva::max_secondary_diagonal_sum(rows, cols, matrix);
  } else if (function_id == 2) {
    result = loseva::count_local_maximums(rows, cols, matrix);
  }
  
  delete[] matrix;
  
  std::ofstream output(output_file);
  output << result;
}

int main(int argc, char* argv[])
{
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <function_id> <input_file> <output_file>\n";
    std::cerr << "  function_id: 1 - max secondary diagonal sum, 2 - count local maximums\n";
    return 1;
  }

  char* end;
  long function_id = std::strtol(argv[1], &end, 10);
  if (*end != '0' || function_id < 1 || function_id > 2) {
    std::cerr << "First parameter is incorrect\n";
    std::cerr << "Must be 1 or 2\n";
    return 1;
  }

  try {
    process_matrix(function_id, argv[2], argv[3]);
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 2;
  }
}
