#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>

bool checkArgc(int argc)
{
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return false;
  }
  if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return false;
  }
  return true;
}

bool checkArgv(char* argv[])
{
  int arg;
  try {
    std::size_t pos = 0;
    arg = std::stoi(argv[1], &pos);
    if (pos < strlen(argv[1])) {
      std::cerr << "First parameter is not a number\n";
      return false;
    }
    if (arg != 1 && arg != 2) {
      std::cerr << "First parameter is out of range\n";
      return false;
    }
  }
  catch (const std::invalid_argument&) {
    std::cerr << "First parameter is not a number\n";
    return false;
  }
  catch (const std::out_of_range&) {
    std::cerr << "First parameter is too big\n";
    return false;
  }
  return true;
}

bool transformFixMatrix(std::ifstream& inputFile, std::ofstream& outputFile, size_t rows, size_t cols)
{
  const int MAX_MATRIX_SIZE = 10000;
  int num;
  int fixMatrix[MAX_MATRIX_SIZE];
  size_t c = 0;
  while (inputFile >> num) {
    fixMatrix[c] = num;
    c++;
  }
  if (inputFile.fail() && !inputFile.eof()) {
    std::cerr << "Error input\n";
    return false;
  }
  if (rows * cols != c) {
    std::cerr << "Incorrect number of parameters\n";
    return false;
  }
  if (rows == 0 || cols == 0) {
    outputFile << 0;
    return true;
  }

  int diagSum[MAX_MATRIX_SIZE] = {};
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      diagSum[i + j] += fixMatrix[i * cols + j];
    }
  }
  int minSum = diagSum[0];
  for (size_t i = 0; i < rows + cols - 1; i++) {
    if (minSum > diagSum[i]) {
      minSum = diagSum[i];
    }
  }
  outputFile << minSum << "\n";
  return true;
}

bool transformDynamicMatrix(std::ifstream& inputFile, std::ofstream& outputFile, size_t rows, size_t cols)
{
  int num;
  int* dynamicMatrix = new int[rows * cols];
  size_t c = 0;
  while (inputFile >> num) {
    dynamicMatrix[c] = num;
    c++;
  }
  if (rows * cols != c) {
    delete[] dynamicMatrix;
    std::cerr << "Incorrect number of parameters\n";
    return false;
  }
  if (inputFile.fail() && !inputFile.eof()) {
    delete[] dynamicMatrix;
    std::cerr << "Error input\n";
    return false;
  }
  if (rows == 0 || cols == 0) {
    outputFile << rows << " " << cols << "\n";
    delete[] dynamicMatrix;
    return true;
  }
  size_t top = 0;
  size_t bottom = rows - 1;
  size_t left = 0;
  size_t right = cols - 1;
  int value = 1;
  while (top <= bottom && left <= right) {
    if (top <= bottom) {
      for (size_t j = left; j <= right; j++) {
        dynamicMatrix[bottom * cols + j] += value;
        value++;
      }
      bottom--;
    }
    if (left <= right) {
      for (size_t i = bottom; i >= top; i--) {
        dynamicMatrix[i * cols + right] += value;
        value++;
      }
      right--;
    }
    if (top <= bottom) {
      for (size_t j = right; j >= left; j--) {
        dynamicMatrix[top * cols + j] += value;
        value++;
      }
      top++;
    }
    if (left <= right) {
      for (size_t i = top; i <= bottom; i++) {
        dynamicMatrix[i * cols + left] += value;
        value++;
      }
      left++;
    }
  }
  outputFile << rows << " " << cols << " ";
  for (size_t i = 0; i < rows * cols - 1; i++) {
    outputFile << dynamicMatrix[i] << " ";
  }
  outputFile << dynamicMatrix[rows * cols - 1] << "\n";
  delete[] dynamicMatrix;
  return true;
}

int main(int argc, char* argv[])
{
  if (!checkArgc(argc) || !checkArgv(argv)) {
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
    std::cerr << "Error rows";
    return 2;
  }
  if (!(inputFile >> cols)) {
    std::cerr << "Error cols";
    return 2;
  }
  if (std::stoi(argv[1]) == 1) {
    if (!transformFixMatrix(inputFile, outputFile, rows, cols)) {
      return 2;
    }
  }
  else if (std::stoi(argv[1]) == 2) {
    if (!transformDynamicMatrix(inputFile, outputFile, rows, cols)) {
      return 2;
    }
  }
  inputFile.close();
  outputFile.close();
  return 0;
}

