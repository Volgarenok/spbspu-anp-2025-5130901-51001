#include <iostream>
#include <fstream>

namespace krivoshapov {

int findColLongestSeries(const int* mat, size_t rows, size_t cols) {
  int best = 1;
  size_t maxLen = 0;

  for (size_t c = 0; c < cols; c++) {
    size_t curRun = 1;
    size_t bestRun = 1;

    for (size_t r = 1; r < rows; r++) {
      if (mat[r * cols + c] == mat[(r - 1) * cols + c]) {
        curRun++;
      } else {
        if (curRun > bestRun) bestRun = curRun;
        curRun = 1;
      }
    }
    
    if (curRun > bestRun) bestRun = curRun;
    
    if (bestRun > maxLen) {
      maxLen = bestRun;
      best = static_cast<int>(c) + 1;
    }
  }

  return best;
}

int countSaddle(const int* mat, size_t rows, size_t cols) {
  int* rowMin = new(std::nothrow) int[rows];
  int* colMax = new(std::nothrow) int[cols];

  if (!rowMin || !colMax) {
    delete[] rowMin;
    delete[] colMax;
    return -1;
  }

  for (size_t r = 0; r < rows; r++) {
    rowMin[r] = mat[r * cols];
  }

  for (size_t c = 0; c < cols; c++) {
    colMax[c] = mat[c];
  }

  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      int val = mat[r * cols + c];
      if (val < rowMin[r]) rowMin[r] = val;
      if (val > colMax[c]) colMax[c] = val;
    }
  }

  int count = 0;
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      int val = mat[r * cols + c];
      if (val == rowMin[r] && val == colMax[c]) count++;
    }
  }

  delete[] rowMin;
  delete[] colMax;
  return count;
}

int run(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Ошибка: неверное количество аргументов\n";
    return 1;
  }

  const char *modeStr = argv[1];
  int mode = 0;

  if (modeStr[0] == '1' && modeStr[1] == 0) mode = 1;
  else if (modeStr[0] == '2' && modeStr[1] == 0) mode = 2;
  else {
    std::cerr << "Ошибка: параметр должен быть 1 или 2\n";
    return 1;
  }

  const char *input = argv[2];
  const char *output = argv[3];

  std::ifstream inFile(input);
  if (!inFile) {
    std::cerr << "Ошибка: не удалось открыть исходный файл\n";
    return 2;
  }

  size_t rows = 0, cols = 0;
  if (!(inFile >> rows >> cols)) {
    std::cerr << "Ошибка: не удалось прочитать размер матрицы\n";
    return 2;
  }

  if (rows == 0 && cols == 0) {
    std::ofstream outFile(output);
    if (!outFile) {
      std::cerr << "Ошибка: не удалось открыть выходной файл\n";
      return 2;
    }
    outFile << 0 << '\n' << 0 << '\n';
    return 0;
  }

  const size_t maxTotal = 10000;
  const size_t total = rows * cols;

  if (mode == 1 && total > maxTotal) {
    std::cerr << "Ошибка: слишком большой массив\n";
    return 2;
  }

  int *mat = nullptr;
  int fixed[maxTotal];

  if (mode == 1) {
    mat = fixed;
  } else {
    mat = new int[total];
  }

  for (size_t i = 0; i < total; i++) {
    if (!(inFile >> mat[i])) {
      if (mode == 2) delete[] mat;
      std::cerr << "Ошибка: файл не содержит достаточного количества чисел\n";
      return 2;
    }
  }

  int best = findColLongestSeries(mat, rows, cols);
  int saddle = countSaddle(mat, rows, cols);

  if (saddle < 0) {
    if (mode == 2) delete[] mat;
    std::cerr << "Ошибка: не удалось выделить память\n";
    return 2;
  }

  std::ofstream outFile(output);
  if (!outFile) {
    if (mode == 2) delete[] mat;
    std::cerr << "Ошибка: не удалось открыть выходной файл\n";
    return 2;
  }

  outFile << best << '\n' << saddle << '\n';

  if (mode == 2) delete[] mat;
  return 0;
}

} 

int main(int argc, char *argv[]) {
  return krivoshapov::run(argc, argv);
}