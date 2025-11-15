#include <iostream>
#include <fstream>
#include <string>

namespace krivoshapov {

int main(int argc, char *argv[])
{
  if (argc != 4) {
    std::cerr << "Ошибка: неверное количество аргументов\n";
    return 1;
  }

  std::string modeStr = argv[1];
  int mode = 0;

  if (modeStr == "1") {
    mode = 1;
  } else if (modeStr == "2") {
    mode = 2;
  } else {
    std::cerr << "Ошибка: параметр должен быть 1 или 2\n";
    return 1;
  }

  const char *inputFile = argv[2];
  const char *outputFile = argv[3];

  std::ifstream fin(inputFile);
  if (!fin) {
    std::cerr << "Ошибка: не удалось открыть исходный файл\n";
    return 2;
  }

  int rows = 0;
  int cols = 0;

  if (!(fin >> rows >> cols)) {
    std::cerr << "Ошибка: не удалось прочитать размер матрицы\n";
    return 2;
  }

  if (rows < 0 || cols < 0) {
    std::cerr << "Ошибка: размеры матрицы не могут быть отрицательными\n";
    return 2;
  }

  if (rows == 0 && cols == 0) {
    std::ofstream fout(outputFile);
    if (!fout) {
      std::cerr << "Ошибка: не удалось открыть выходной файл\n";
      return 2;
    }
    fout << 0 << '\n' << 0 << '\n';
    return 0;
  }

  const int maxSize = 10000;
  const int total = rows * cols;

  if (mode == 1 && total > maxSize) {
    std::cerr << "Ошибка: слишком большой массив\n";
    return 2;
  }

  int *data = nullptr;
  int staticArr[maxSize];

  if (mode == 1) {
    data = staticArr;
  } else {
    data = new int[total];
  }

  for (int i = 0; i < total; i++) {
    if (!(fin >> data[i])) {
      if (mode == 2) {
        delete[] data;
      }
      std::cerr << "Ошибка: файл не содержит достаточного количества чисел\n";
      return 2;
    }
  }

  // ВАРИАНТ 11 — поиск столбца с самой длинной серией повторов
  int bestCol = 1;
  int bestLen = 0;

  for (int c = 0; c < cols; c++) {
    int maxRun = 1;
    int curRun = 1;

    for (int r = 1; r < rows; r++) {
      const int prev = data[(r - 1) * cols + c];
      const int cur = data[r * cols + c];

      if (cur == prev) {
        curRun++;
      } else {
        if (curRun > maxRun) {
          maxRun = curRun;
        }
        curRun = 1;
      }
    }

    if (curRun > maxRun) {
      maxRun = curRun;
    }

    if (maxRun > bestLen) {
      bestLen = maxRun;
      bestCol = c + 1;
    }
  }

  // ВАРИАНТ 1 — седловые точки
  int *rowMin = new int[rows];
  int *colMax = new int[cols];

  for (int r = 0; r < rows; r++) {
    rowMin[r] = data[r * cols];
  }

  for (int c = 0; c < cols; c++) {
    colMax[c] = data[c];
  }

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      const int val = data[r * cols + c];

      if (val < rowMin[r]) {
        rowMin[r] = val;
      }
      if (val > colMax[c]) {
        colMax[c] = val;
      }
    }
  }

  int saddle = 0;

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      const int val = data[r * cols + c];

      if (val == rowMin[r] && val == colMax[c]) {
        saddle++;
      }
    }
  }

  std::ofstream fout(outputFile);
  if (!fout) {
    if (mode == 2) {
      delete[] data;
    }
    delete[] rowMin;
    delete[] colMax;

    std::cerr << "Ошибка: не удалось открыть выходной файл\n";
    return 2;
  }

  fout << bestCol << '\n' << saddle << '\n';

  if (mode == 2) {
    delete[] data;
  }

  delete[] rowMin;
  delete[] colMax;

  return 0;
}

}  // namespace krivoshapov

int main(int argc, char *argv[])
{
  return krivoshapov::main(argc, argv);
}
