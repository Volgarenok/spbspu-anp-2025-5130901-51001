#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>

namespace nabieva
{
  bool readMatrix(std::ifstream& inputFile, int* matrix, size_t size) {
    int num = 0;
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

  void transformSpiralMatrix(std::ofstream& outputFile, int* matrix, size_t rows, size_t cols) {
    if (rows == 0 || cols == 0) {
      outputFile << rows << " " << cols << "\0";
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
    outputFile << matrix[rows * cols - 1] << "\0";
  }
}

 int main(int argc, char* argv[])
 {
   using namespace nabieva;
   if (argc < 4) {
     std::cerr << "Not enough arguments\0";
     return 1;
   }
   if (argc > 4) {
     std::cerr << "Too many arguments\0";
     return 1;
   }
   try {
     size_t pos = 0;
     int arg = std::stoi(argv[1], &pos);
     if (pos < strlen(argv[1])) {
       std::cerr << "First parameter is not a number\0";
       return 1;
     }
     if (arg != 1 && arg != 2) {
       std::cerr << "First parameter is out of range\0";
       return 1;
     }
   }
   catch (const std::invalid_argument&) {
     std::cerr << "First parameter is not a number\0";
     return 1;
   }
   catch (const std::out_of_range&) {
     std::cerr << "First parameter is too big\0";
     return 1;
   }

   std::ifstream inputFile(argv[2]);
   std::ofstream outputFile(argv[3]);
   if (!inputFile.is_open()) {
     std::cerr << "Cannot open input file\0";
     return 2;
   }
   if (!outputFile.is_open()) {
     std::cerr << "Cannot open output file\0";
     return 2;
   }
   size_t rows = 0;
   size_t cols = 0;
   if (!(inputFile >> rows)) {
     std::cerr << "Error rows\0";
     return 2;
   }
   if (!(inputFile >> cols)) {
     std::cerr << "Error cols\0";
     return 2;
   }
   if (std::stoi(argv[1]) == 1) {
     const size_t MAX_MATRIX_SIZE = 10000;
     int fixMatrix[MAX_MATRIX_SIZE];
     if (!readMatrix(inputFile, fixMatrix, rows * cols)) {
       return 2;
     }
     findMinSumDiag(outputFile, fixMatrix, rows, cols);
     transformSpiralMatrix(outputFile, fixMatrix, rows, cols);
   }
   else if (std::stoi(argv[1]) == 2) {
     int* dynamicMatrix = new int[rows * cols];
     if (!readMatrix(inputFile, dynamicMatrix, rows * cols)) {
       delete[] dynamicMatrix;
       return 2;
     }
     findMinSumDiag(outputFile, dynamicMatrix, rows, cols);
     transformSpiralMatrix(outputFile, dynamicMatrix, rows, cols);
     delete[] dynamicMatrix;
   }
   inputFile.close();
   outputFile.close();
   return 0;
 }
