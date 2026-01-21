#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>

namespace gordejchik
{
  bool readMatrix(std::ifstream& inputFile, int* matrix, size_t size) {
    int num;
    size_t c = 0;
    while (inputFile >> num) {
      matrix[c] = num;
      c++;
    }
    if (inputFile.fail() && !inputFile.eof()) {
      std::cerr << "Error input\0";
      return false;
    }
    if (size != c) {
      std::cerr << "Incorrect number of parameters\0";
      return false;
    }
    return true;
  }
  
  void transformMatrix(std::ofstream& outputFile, int* matrix, size_t rows, size_t cols) {
      if (rows == 0 || cols == 0) {
        outputFile << rows << " " << cols << "\n";
        return;
      }
      ptrdiff_t top = 0;
      ptrdiff_t bottom = rows - 1;
      ptrdiff_t left = 0;
      ptrdiff_t right = cols - 1;
      int value = 1;
      while (top <= bottom && left <= right) {
        if (top <= bottom) {
          for (size_t j = left; static_cast<int>(j) <= right; j++) {
            matrix[bottom * cols + j] += value;
            value++;
          }
          bottom--;
        }
        if (left <= right) {
          for (size_t i = bottom; static_cast<int>(i) >= top; i--) {
            matrix[i * cols + right] += value;
            value++;
          }
          right--;
        }
        if (top <= bottom) {
          for (size_t j = right; static_cast<int>(j) >= left; j--) {
            matrix[top * cols + j] += value;
            value++;
          }
          top++;
        }
        if (left <= right) {
          for (size_t i = top; static_cast<int>(i) <= bottom; i++) {
            matrix[i * cols + left] += value;
            value++;
          }
          left++;
        }
      }
      outputFile << rows << " " << cols << " ";
      for (size_t i = 0; i < rows * cols - 1; i++) {
        outputFile << matrix[i] << " ";
      }
      outputFile << matrix[rows * cols - 1] << "\n";
    }
    
  void findMinSumDiag(std::ofstream& outputFile, int* matrix, size_t rows, size_t cols) {
      if (rows == 0 || cols == 0) {
        outputFile << 0 << "\n";
        return;
      }
      const size_t MAX_MATRIX_SIZE = 10000;
      int diagSum[MAX_MATRIX_SIZE] = {};
      for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
          diagSum[i + j] += matrix[i * cols + j];
        }
      }
      int minSum = diagSum[0];
      for (size_t i = 0; i < rows + cols - 1; i++) {
        if (minSum > diagSum[i]) {
          minSum = diagSum[i];
        }
      }
      outputFile << minSum << "\n";
    }
}
