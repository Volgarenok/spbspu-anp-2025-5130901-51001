#include <fstream>
#include <iostream>

// 8.[FLL-INC-WAV]
void task1_processMatrix(int **matrix, int rows, int cols) {
  // Количество слоев — это половина от меньшей стороны
  // (если нечётно — ещё один слой)
  int layers = std::min(rows, cols) / 2 + (std::min(rows, cols) % 2);

  for (int layer = 0; layer < layers; ++layer) {
    // На каждом слое прибавляем число слоя
    // (1 на внешнем, 2 на следующем ит.д.)
    int increment = layer + 1;

    // Границы текущего слоя
    int top = layer;
    int bottom = rows - 1 - layer;
    int left = layer;
    int right = cols - 1 - layer;

    // Пробегаем по верхней строке слоя
    for (int j = left; j <= right; ++j)
      matrix[top][j] += increment;

    // Нижняя строка, если она не совпала с верхней
    if (top != bottom)
      for (int j = left; j <= right; ++j)
        matrix[bottom][j] += increment;

    // Левая вертикаль слоя
    for (int i = top + 1; i < bottom; ++i)
      matrix[i][left] += increment;

    // Правая вертикаль, если не совпала с левой
    if (left != right)
      for (int i = top + 1; i < bottom; ++i)
        matrix[i][right] += increment;
  }
}

// 18.[CNT-NZR-DIG]
// Считаем, сколько диагоналей содержат
// только ненулевые элементы.
int task2_countNonZeroDiagonals(int **matrix, int rows, int cols) {
  int count = 0;

  // Запускаем диагонали от первой строки
  for (int startCol = 0; startCol < cols; ++startCol) {
    bool ok = true;
    int i = 0, j = startCol;

    // Идём по диагонали вниз-вправо
    while (i < rows && j < cols) {
      if (matrix[i][j] == 0) {
        ok = false;
        break;
      }
      i++;
      j++;
    }
    if (ok)
      count++;
  }

  // Запускаем диагонали с первого столбца,
  // начиная со второй строки
  for (int startRow = 1; startRow < rows; ++startRow) {
    bool ok = true;
    int i = startRow, j = 0;

    while (i < rows && j < cols) {
      if (matrix[i][j] == 0) {
        ok = false;
        break;
      }
      i++;
      j++;
    }
    if (ok)
      count++;
  }

  return count;
}

// Читаем матрицу из файла.
// Если что-то пошло не так — возвращаем false.
bool readMatrix(int **matrix, int rows, int cols, std::ifstream &fin) {
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      if (!(fin >> matrix[i][j]))
        return false;
  return true;
}

// Записываем матрицу в файл
void writeMatrix(int **matrix, int rows, int cols, std::ofstream &fout) {
  fout << rows << " " << cols << "\n";
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      fout << matrix[i][j] << " ";
}

// Универсальное освобождение матрицы
void freeMatrix(int **matrix, int rows, int mode) {
  if (!matrix)
    return;

  if (mode == 1) {
    // Статический буфер: освобождаем только массив указателей
    delete[] matrix;
  } else if (mode == 2) {
    // Динамический: освобождаем каждую строку и массив указателей
    for (int i = 0; i < rows; ++i)
      delete[] matrix[i];
    delete[] matrix;
  }
}

int main(int argc, char *argv[]) {

  // Проверяем количество аргументов
  if (argc != 4) {
    std::cerr << "Invalid number of arguments\n";
    return 1;
  }

  // Проверяем, что первый аргумент — это либо 1, либо 2
  if ((argv[1][0] != '1' && argv[1][0] != '2') || argv[1][1] != '\0') {
    std::cerr << "First argument is out of range or not a number\n";
    return 1;
  }

  int num = argv[1][0] - '0';

  // Открываем файлы ввода и вывода
  std::ifstream fin(argv[2]);
  if (!fin) {
    std::cerr << "Cannot open input file\n";
    return 2;
  }

  std::ofstream fout(argv[3]);
  if (!fout) {
    std::cerr << "Cannot open output file\n";
    return 2;
  }

  // Считываем размеры матрицы
  int rows, cols;
  if (!(fin >> rows >> cols)) {
    std::cerr << "Invalid matrix format\n";
    return 2;
  }
  if (rows < 0 || cols < 0) {
    std::cerr << "Invalid matrix dimensions\n";
    return 2;
  }

  int **matrix = nullptr;

  // Выделяем память (статически или динамически)
  if (num == 1) {
    // Ограничение для статического режима
    if (rows * cols > 10000) {
      std::cerr << "Matrix too large for static mode\n";
      return 2;
    }

    static int buffer[100][100];
    matrix = new int *[rows];
    for (int i = 0; i < rows; ++i)
      matrix[i] = buffer[i];
  } else { // Динамическое выделение
    matrix = new (std::nothrow) int *[rows];
    if (!matrix) {
      std::cerr << "Memory allocation error\n";
      return 2;
    }

    for (int i = 0; i < rows; ++i) {
      matrix[i] = new (std::nothrow) int[cols];
      if (!matrix[i]) {
        std::cerr << "Memory allocation error\n";
        // Освобождаем уже выделенные строки и массив указателей
        for (int k = 0; k < i; ++k)
          delete[] matrix[k];
        delete[] matrix;
        return 2;
      }
    }
  }

  // Читаем матрицу
  if (!readMatrix(matrix, rows, cols, fin)) {
    std::cerr << "Invalid matrix content\n";
    // Освобождаем память перед выходом
    freeMatrix(matrix, rows, num);
    return 2;
  }

  // Выполняем одно из двух заданий
  if (num == 1) {
    task1_processMatrix(matrix, rows, cols);
    writeMatrix(matrix, rows, cols, fout);
  } else if (num == 2) {
    int result = task2_countNonZeroDiagonals(matrix, rows, cols);
    fout << result;
  }

  // Освобождаем память во всех случаях
  freeMatrix(matrix, rows, num);

  return 0;
}
