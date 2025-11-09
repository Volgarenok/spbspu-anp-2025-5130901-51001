#include <iostream>
#include <fstream>

namespace shaykhraziev {
  void lft_bot_cnt(int* data, size_t rows, size_t cols);
  int min_sum_sdg(const int* data, size_t rows, size_t cols);
  void readMatrix(std::istream& in, int* data, size_t rows, size_t cols);
  void writeMatrix(std::ostream& out, const int* data, size_t rows, size_t cols);
  void writeResult(std::ostream& out, int result);
}

void shaykhraziev::lft_bot_cnt(int* data, size_t rows, size_t cols) {
  if (rows == 0 || cols == 0) {
    return;
  }
  size_t minDim = rows < cols ? rows : cols;
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

int shaykhraziev::min_sum_sdg(const int* data, size_t rows, size_t cols) {
  if (rows == 0 || cols == 0) {
    return 0;
  }
  int minSum = data[cols - 1];
  for (size_t startCol = 0; startCol < cols; ++startCol) {
    int sum = 0;
    size_t i = 0;
    size_t j = startCol;
    while (i < rows && j < cols) {
      sum += data[i * cols + j];
      ++i;
      ++j;
    }
    if (sum < minSum) {
      minSum = sum;
    }
  }

  for (size_t startRow = 1; startRow < rows; ++startRow) {
    int sum = 0;
    size_t i = startRow;
    size_t j = 0;
    while (i < rows && j < cols) {
      sum += data[i * cols + j];
      ++i;
      ++j;
    }
    if (sum < minSum) {
      minSum = sum;
    }
  }
  return minSum;
}

void shaykhraziev::readMatrix(std::istream& in, int* data, size_t rows, size_t cols) {
  if (rows == 0 || cols == 0) {
    return;
  }
  size_t total = rows * cols;
  for (size_t k = 0; k < total; ++k) {
    if (!(in >> data[k])) {
      throw std::runtime_error("readMatrix failed: not enough data");
    }
  }
  std::string extra;
  if (in >> extra) {
    throw std::runtime_error("readMatrix failed: extra data");
  }
}

void shaykhraziev::writeMatrix(std::ostream& out, const int* data, size_t rows, size_t cols) {
  out << rows << " " << cols;
  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < cols; ++c) {
      out << " " << data[r * cols + c];
    }
  }
}

void shaykhraziev::writeResult(std::ostream& out, int result) {
  out << result;
}

int main(int argc, char* argv[]) {
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
    std::cerr << "readMatrix failed";
    return 2;
  }

  size_t rows = 0, cols = 0;
  if (!(fin >> rows >> cols)) {
    std::cerr << "readMatrix failed";
    return 2;
  }

  size_t total = rows * cols;
  int* data = nullptr;
  if (total > 0) {
    try {
      data = new int[total];
    } catch (...) {
      std::cerr << "readMatrix failed";
      return 2;
    }
    try {
      shaykhraziev::readMatrix(fin, data, rows, cols);
    } catch (const std::exception& e) {
      std::cerr << e.what();
      delete[] data;
      return 2;
    }
  }

  std::ofstream fout(outputFile);
  if (!fout.is_open()) {
    std::cerr << "readMatrix failed";
    delete[] data;
    return 2;
  }

  if (taskNum == 1) {
    shaykhraziev::lft_bot_cnt(data, rows, cols);
    shaykhraziev::writeMatrix(fout, data, rows, cols);
  } else {
    int result = shaykhraziev::min_sum_sdg(data, rows, cols);
    shaykhraziev::writeResult(fout, result);
  }

  delete[] data;
  return 0;
}
