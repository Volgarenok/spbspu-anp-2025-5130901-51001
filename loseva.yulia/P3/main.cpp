#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
namespace loseva
{
  bool is_local_maximum(size_t r, size_t c, size_t rows, size_t cols, const int* matrix)
  {
    int current = matrix[r * cols + c];
    int offset[8][2] = {
      {-1, -1}, {-1, 0}, {-1, 1},
      { 0, -1},          { 0, 1},
      { 1, -1}, { 1, 0}, { 1, 1}
    };
    for (int k = 0; k < 8; ++k) {
      size_t ni = r + offset[k][0];
      size_t nj = c + offset[k][1];
      if (matrix[ni * cols + nj] >= current) {
        return false;
      }
    }
    return true;
  }
  int max_secondary_diagonal_sum(size_t rows, size_t cols, const int* matrix)
  {
    if (rows == 0 || cols == 0) return 0;
    int maxSum = 0;
    bool firstSumSet = false;
    int n = static_cast<int>(std::min(rows, cols));
    for (int diagIndex = 0; diagIndex <= 2 * (n - 1); ++diagIndex) {
      if (diagIndex == n - 1) continue;
      int diagSum = 0;
      bool hasElements = false;
      for (int i = 0; i < n; ++i) {
        int j = diagIndex - i;
        if (j >= 0 && j < n) {
          diagSum += matrix[i * cols + j];
          hasElements = true;
        }
      }
      if (hasElements) {
        if (!firstSumSet || diagSum > maxSum) {
          maxSum = diagSum;
          firstSumSet = true;
        }
      }
    }
    return maxSum;
  }
  int count_local_maximums(size_t rows, size_t cols, const int* matrix)
  {
    if (rows < 3 || cols < 3) return 0;
    int count = 0;
    for (size_t i = 1; i < rows - 1; i++) {
      for (size_t j = 1; j < cols - 1; j++) {
        if (is_local_maximum(i, j, rows, cols, matrix)) {
          count++;
        }
      }
    }
    return count;
  }
  int read_matrix_from_file(const std::string& filename, int*& matrix, size_t& rows, size_t& cols)
  {
    std::ifstream input(filename);
    if (!input.is_open()) return -1;
    if (!(input >> rows >> cols)) return -2;
    if (rows == 0 || cols == 0) return -2;
    if (rows > 10000 || cols > 10000) return -2;
    matrix = new int[rows * cols];
    for (size_t i = 0; i < rows * cols; i++) {
      if (!(input >> matrix[i])) {
        delete[] matrix;
        matrix = nullptr;
        return -3;
      }
    }
    if (input >> extra) {
      delete[] matrix;
      matrix = nullptr;
      return -4;
    }
    return 0;
  }
  void write_result_to_file(const std::string& filename, int result)
  {
    std::ofstream output(filename);
    if (output.is_open()) {
      output << result << "\n";
    }
  }
}
void processMatrix(int functionId, const char* inputFile, const char* outputFile)
{
  int* matrix = nullptr;
  size_t rows = 0, cols = 0;
  int readStatus = loseva::read_matrix_from_file(inputFile, matrix, rows, cols); 
  if (readStatus != 0) {
    if (matrix) delete[] matrix;
    if (readStatus == -4) throw std::runtime_error("Extra data in file");
    throw std::runtime_error("Failed to read matrix or invalid data");
  int result = 0;
  if (functionId == 1) {
    result = loseva::max_secondary_diagonal_sum(rows, cols, matrix);
  } else {
    result = loseva::count_local_maximums(rows, cols, matrix);
  }
  delete[] matrix;
  loseva::write_result_to_file(outputFile, result);
}
int main(int argc, char* argv[])
{
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <id> <input> <output>\n";
    return 1;
  }
  char* end;
  long functionId = std::strtol(argv[1], &end, 10);
  bool isParamValid = (*end == '\0');
  bool isIdInRange = (functionId == 1 || functionId == 2);
  if (!isParamValid || !isIdInRange) {
    std::cerr << "Invalid function ID. Use 1 or 2.\n";
    return 1;
  }
  try {
    processMatrix(static_cast<int>(functionId), argv[2], argv[3]);
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 2;
  }
}
