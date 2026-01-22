#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>

namespace nabieva
{
  size_t readMatrix(std::ifstream& inputFile, int* matrix)
  {
    int num = 0;
    size_t c = 0;
    while (inputFile >> num) {
      matrix[c] = num;
      c++;
    }
    if (inputFile.fail() && !inputFile.eof()) {
      throw std::runtime_error("Error input\n");
    }
    return c;
  }

  int findMinSumDiag(const int* matrix, size_t rows, size_t cols)
  {
    int minSum = 0;
    if (rows == 0 || cols == 0) {
      return minSum;
    }
    const size_t MAX_MATRIX_SIZE = 10000;
    int diagSum[MAX_MATRIX_SIZE] = {};
    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < cols; j++) {
        diagSum[i + j] += matrix[i * cols + j];
      }
    }
    minSum = diagSum[0];
    for (size_t i = 0; i < rows + cols - 1; i++) {
      if (minSum > diagSum[i]) {
        minSum = diagSum[i];
      }
    }
    return minSum;
  }

  void transformSpiralMatrix(std::ofstream& outputFile, int* matrix, size_t rows, size_t cols)
  {
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
        for (size_t j = left; static_cast<ptrdiff_t>(j) <= right; j++) {
          matrix[bottom * cols + j] += value;
          value++;
        }
        bottom--;
      }
      if (left <= right) {
        for (size_t i = bottom; static_cast<ptrdiff_t>(i) >= top; i--) {
          matrix[i * cols + right] += value;
          value++;
        }
        right--;
      }
      if (top <= bottom) {
        for (size_t j = right; static_cast<ptrdiff_t>(j) >= left; j--) {
          matrix[top * cols + j] += value;
          value++;
        }
        top++;
      }
      if (left <= right) {
        for (size_t i = top; static_cast<ptrdiff_t>(i) <= bottom; i++) {
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
}

 int main(int argc, char* argv[])
 {
   using namespace nabieva;
   if (argc < 4) {
     std::cerr << "Not enough arguments\n";
     return 1;
   }
   if (argc > 4) {
     std::cerr << "Too many arguments\n";
     return 1;
   }
   try {
     size_t pos = 0;
     int arg = std::stoi(argv[1], &pos);
     if (pos < strlen(argv[1])) {
       std::cerr << "First parameter is not a number\n";
       return 1;
     }
     if (arg != 1 && arg != 2) {
       std::cerr << "First parameter is out of range\n";
       return 1;
     }
   }
   catch (const std::invalid_argument&) {
     std::cerr << "First parameter is not a number\n";
     return 1;
   }
   catch (const std::out_of_range&) {
     std::cerr << "First parameter is too big\n";
     return 1;
   }

   std::ifstream inputFile(argv[2]);
   std::ofstream outputFile(argv[3]);
   if (!inputFile.is_open()) {
     std::cerr << "Cannot open input file\n";
     return 2;
   }
   if (!outputFile.is_open()) {
     std::cerr << "Cannot open output file\n";
     return 2;
   }
   size_t rows = 0;
   size_t cols = 0;
   if (!(inputFile >> rows)) {
     std::cerr << "Error rows\n";
     return 2;
   }
   if (!(inputFile >> cols)) {
     std::cerr << "Error cols\n";
     return 2;
   }
   if (std::stoi(argv[1]) == 1) {
     const size_t MAX_MATRIX_SIZE = 10000;
     int fixMatrix[MAX_MATRIX_SIZE];
     size_t inputSize = 0;
     if (!(inputSize = readMatrix(inputFile, fixMatrix))) {
       std::cerr << "Error input\n";
       return 2;
     }
     if (cols * rows != inputSize) {
       std::cerr << "Incorrect number of parameters\n";
       return 2;
     }
     outputFile << findMinSumDiag(fixMatrix, rows, cols) << "\n";
     transformSpiralMatrix(outputFile, fixMatrix, rows, cols);
   }
   else if (std::stoi(argv[1]) == 2) {
     int* dynamicMatrix = new int[rows * cols];
     size_t inputSize = 0;
     if (!(inputSize = readMatrix(inputFile, dynamicMatrix))) {
       std::cerr << "Error input\n";
       delete[] dynamicMatrix;
       return 2;
     }
     if (cols * rows != inputSize) {
       std::cerr << "Incorrect number of parameters\n";
       return 2;
     }
     outputFile << findMinSumDiag(dynamicMatrix, rows, cols) << "\n";
     transformSpiralMatrix(outputFile, dynamicMatrix, rows, cols);
     delete[] dynamicMatrix;
   }
   inputFile.close();
   outputFile.close();
   return 0;
 }
