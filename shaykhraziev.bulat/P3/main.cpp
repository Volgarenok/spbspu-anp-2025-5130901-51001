#include <iostream>
#include <fstream>

namespace shaykhraziev
{
  void incrementCounterclockwise(int* data, size_t rows, size_t cols);
  int findMinSum(const int* data, size_t sideSize, size_t iCols);
  size_t readMatrix(std::istream& in, int* data, size_t rows, size_t cols);
  void writeResult(std::ostream& out, const int* data, size_t rows, size_t cols);
}

void shaykhraziev::incrementCounterclockwise(int* data, size_t rows, size_t cols)
{
  if (rows == 0 || cols == 0) {
    return;
  }

  size_t minDim = std::min(rows, cols);
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
}

int main(int argc, char* argv[])
{
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }

  if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }

  int taskNum = 0;

  try {
    taskNum = std::stoi(argv[1]);
  } catch (...) {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }

  if (taskNum != 1 && taskNum != 2) {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }

  const char* inputFile = argv[2];
  const char* outputFile = argv[3];

  std::ifstream fin(inputFile);

  if (!fin.is_open()) {
    std::cerr << "open input file failed\n";
    return 2;
  }

  size_t rows = 0;
  size_t cols = 0;

  if (!(fin >> rows >> cols)) {
    std::cerr << "read matrix failed\n";
    return 2;
  }

  size_t total = rows * cols;

  int* data = nullptr;
  int fixed_size_arr_d[10000] = {};

  if (total > 0 && taskNum == 2) {
    try {
      data = new int[total];
    } catch (const std::bad_alloc& e) {
      std::cerr << "memory allocation failed: " << e.what() << "\n";
      return 2;
    }
  }

  int* d = (taskNum == 1) ? fixed_size_arr_d : data;

  if (shaykhraziev::readMatrix(fin, d, rows, cols) != total) {
    std::cerr << "read matrix failed\n";
    delete[] data;
    return 2;
  }

  std::ofstream fout(outputFile);

  if (!fout.is_open()) {
    std::cerr << "open output file failed\n";
    delete[] data;
    return 2;
  }

  shaykhraziev::incrementCounterclockwise(d, rows, cols);
  shaykhraziev::writeResult(fout, d, rows, cols);

  size_t sideSize = std::min(rows, cols);
  int result = shaykhraziev::findMinSum(d, sideSize, cols - sideSize);

  fout << result << "\n";

  delete[] data;

  return 0;
}
