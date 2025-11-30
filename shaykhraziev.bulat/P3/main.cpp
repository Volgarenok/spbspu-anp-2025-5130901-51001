#include <iostream>
#include <fstream>

namespace shaykhraziev
{
  void IncrementCounterclockwise(int* data, size_t rows, size_t cols);
  int findMinSum(const int* data, size_t sideSize, size_t iCols);
  size_t readMatrix(std::istream& in, int* data, size_t rows, size_t cols);
  void writeResult(std::ostream& out, const int* data, size_t rows, size_t cols);
}

void shaykhraziev::IncrementCounterclockwise(int* data, size_t rows, size_t cols)
{
  if (rows == 0 || cols == 0) {
    return;
  }

  size_t minDim = (rows < cols) ? rows : cols;
  size_t layers = (minDim + 1) / 2;
  int inc = 1;

  for (size_t layer = 0; layer < layers; ++layer) {
    size_t startRow = rows - 1 - layer;
    size_t endRow = layer;
    size_t startCol = layer;
    size_t endCol = cols - 1 - layer;

    if (startRow < endRow || startCol > endCol) {
      break;
    }

    for (size_t j = startCol; j <= endCol && startRow >= endRow; ++j) {
      data[startRow * cols + j] += inc++;
    }

    if (startCol < endCol) {
      for (size_t i = startRow; i > endRow; --i) {
        data[(i - 1) * cols + endCol] += inc++;
      }
    }

    if (startRow > endRow) {
      for (size_t j = endCol; j > startCol; --j) {
        data[endRow * cols + (j - 1)] += inc++;
      }
    }

    if (startCol < endCol && startRow > endRow + 1) {
      for (size_t i = endRow + 1; i < startRow; ++i) {
        data[i * cols + startCol] += inc++;
      }
    }
  }
}

int shaykhraziev::findMinSum(const int* data, size_t sideSize, size_t iCols)
{
  if (sideSize == 0) {
    return 0;
  }

  int minSum = data[sideSize - 1];

  for (size_t startCol = 0; startCol < sideSize; ++startCol) {
    int sum = 0;
    size_t i = 0;
    size_t j = startCol;

    while (i < sideSize && j < sideSize) {
      size_t shift = iCols * i;
      sum += data[i * sideSize + j + shift];
      ++i;
      ++j;
    }

    if (sum < minSum) {
      minSum = sum;
    }
  }

  for (size_t startRow = 1; startRow < sideSize; ++startRow) {
    int sum = 0;
    size_t i = startRow;
    size_t j = 0;

    while (i < sideSize && j < sideSize) {
      size_t shift = iCols * i;
      sum += data[i * sideSize + j + shift];
      ++i;
      ++j;
    }

    if (sum < minSum) {
      minSum = sum;
    }
  }

  return minSum;
}

size_t shaykhraziev::readMatrix(std::istream& in, int* data, size_t rows, size_t cols)
{
  size_t total = rows * cols;

  for (size_t k = 0; k < total; ++k) {
    if (!(in >> data[k])) {
      return k;
    }
  }

  return total;
}

void shaykhraziev::writeResult(std::ostream& out, const int* data, size_t rows, size_t cols)
{
  out << rows << " " << cols;

  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < cols; ++c) {
      out << " " << data[r * cols + c];
    }
  }

  out << "\n";
}

int main(int argc, char* argv[])
{
  if (argc < 4) {
    std::cerr << "Not enough arguments";
    return 1;
  }

  if (argc > 4) {
    std::cerr << "Too many arguments";
    return 1;
  }

  int taskNum;

  try {
    taskNum = std::stoi(argv[1]);
  } catch (...) {
    std::cerr << "First parameter is not a number";
    return 1;
  }

  if (taskNum != 1 && taskNum != 2) {
    std::cerr << "First parameter is out of range";
    return 1;
  }

  const char* inputFile = argv[2];
  const char* outputFile = argv[3];

  std::ifstream fin(inputFile);

  if (!fin.is_open()) {
    std::cerr << "open input file failed";
    return 2;
  }

  size_t rows = 0;
  size_t cols = 0;

  if (!(fin >> rows >> cols)) {
    std::cerr << "read matrix failed";
    return 2;
  }

  size_t total = rows * cols;
  int* data = nullptr;
  int static_data[10000] = {};

  if (total > 0) {
    try {
      data = new int[total];
    } catch (...) {
      std::cerr << "memory allocation failed";
      return 2;
    }
  }

  int* d = (taskNum == 1) ? static_data : data;

  if (shaykhraziev::readMatrix(fin, d, rows, cols) != total) {
    std::cerr << "read matrix failed";
    delete[] data;
    return 2;
  }

  std::ofstream fout(outputFile);

  if (!fout.is_open()) {
    std::cerr << "open output file failed";
    delete[] data;
    return 2;
  }

  if (taskNum == 1) {
    shaykhraziev::IncrementCounterclockwise(static_data, rows, cols);
    shaykhraziev::writeResult(fout, static_data, rows, cols);

    size_t sideSize = std::min(rows, cols);
    int result = shaykhraziev::findMinSum(static_data, sideSize, cols - sideSize);
    fout << result << "\n";
  } else {
    shaykhraziev::IncrementCounterclockwise(data, rows, cols);
    shaykhraziev::writeResult(fout, data, rows, cols);

    size_t sideSize = std::min(rows, cols);
    int result = shaykhraziev::findMinSum(data, sideSize, cols - sideSize);
    fout << result << "\n";
  }

  delete[] data;

  return 0;
}
