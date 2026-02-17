#include <iostream>
#include <fstream>
namespace krivoshapov {

  int findColumnWithLongestSeries(const int* matrix, size_t rows, size_t columns)
  {
    int best_column = 1;
    size_t max_length = 0;

    for (size_t column = 0; column < columns; ++column) {
      size_t current_run = 1;
      size_t best_run = 1;

      for (size_t row = 1; row < rows; ++row) {
        if (matrix[row * columns + column] == matrix[(row - 1) * columns + column]) {
          ++current_run;
        } else {
          if (current_run > best_run) {
            best_run = current_run;
          }
          current_run = 1;
        }
      }

      if (current_run > best_run) {
        best_run = current_run;
      }

      if (best_run > max_length) {
        max_length = best_run;
        best_column = static_cast< int >(column) + 1;
      }
    }

    return best_column;
  }

  size_t countSaddlePoints(const int* matrix, size_t rows, size_t columns)
   {
    int* row_minimums = nullptr;
    int* column_maximums = nullptr;

    try {
      row_minimums = new int[rows];
      column_maximums = new int[columns];
    } catch (const std::bad_alloc&) {
      delete[] row_minimums;
      delete[] column_maximums;
      throw;
    }

    for (size_t row = 0; row < rows; ++row) {
      row_minimums[row] = matrix[row * columns];
    }

    for (size_t column = 0; column < columns; ++column) {
      column_maximums[column] = matrix[column];
    }

    for (size_t row = 0; row < rows; ++row) {
      for (size_t column = 0; column < columns; ++column) {
        int value = matrix[row * columns + column];
        if (value < row_minimums[row]) {
          row_minimums[row] = value;
        }
        if (value > column_maximums[column]) {
          column_maximums[column] = value;
        }
      }
    }

    size_t count = 0;
    for (size_t row = 0; row < rows; ++row) {
      for (size_t column = 0; column < columns; ++column) {
        int value = matrix[row * columns + column];
        if (value == row_minimums[row] && value == column_maximums[column]) {
          ++count;
        }
      }
    }

    delete[] row_minimums;
    delete[] column_maximums;
    return count;
  }

}

int main(int argc, char* argv[])
{
  if (argc != 4) {
    std::cerr << "Ошибка: неверное количество аргументов\n";
    return 1;
  }

  const char* mode_string = argv[1];
  int mode = 0;

  if (mode_string[0] == '1' && mode_string[1] == 0) {
    mode = 1;
  } else if (mode_string[0] == '2' && mode_string[1] == 0) {
    mode = 2;
  } else {
    std::cerr << "Ошибка: параметр должен быть 1 или 2\n";
    return 1;
  }

  const char* input_file_name = argv[2];
  const char* output_file_name = argv[3];

  std::ifstream input_file(input_file_name);
  if (!input_file) {
    std::cerr << "Ошибка: не удалось открыть исходный файл\n";
    return 2;
  }

  size_t rows = 0;
  size_t columns = 0;
  if (!(input_file >> rows >> columns)) {
    std::cerr << "Ошибка: не удалось прочитать размер матрицы\n";
    return 2;
  }

  if (rows == 0 && columns == 0) {
    std::ofstream output_file(output_file_name);
    if (!output_file) {
      std::cerr << "Ошибка: не удалось открыть выходной файл\n";
      return 2;
    }
    output_file << 0 << '\n' << 0 << '\n';
    return 0;
  }

  const size_t max_total_elements = 10000;
  const size_t total_elements = rows * columns;

  if (mode == 1 && total_elements > max_total_elements) {
    std::cerr << "Ошибка: слишком большой массив\n";
    return 2;
  }

  int* matrix = nullptr;
  int fixed_size_matrix[max_total_elements];

  if (mode == 1) {
    matrix = fixed_size_matrix;
  } else {
    try {
      matrix = new int[total_elements];
    } catch (const std::bad_alloc&) {
      std::cerr << "Ошибка: не удалось выделить память\n";
      return 2;
    }
  }

  for (size_t index = 0; index < total_elements; ++index) {
    if (!(input_file >> matrix[index])) {
      if (mode == 2) {
        delete[] matrix;
      }
      std::cerr << "Ошибка: файл не содержит достаточного количества чисел\n";
      return 2;
    }
  }

  int best_column = krivoshapov::findColumnWithLongestSeries(matrix, rows, columns);
  size_t saddle_points_count = 0;

  try {
    saddle_points_count = krivoshapov::countSaddlePoints(matrix, rows, columns);
  } catch (const std::bad_alloc&) {
    if (mode == 2) {
      delete[] matrix;
    }
    std::cerr << "Ошибка: не удалось выделить память\n";
    return 2;
  }

  std::ofstream output_file(output_file_name);
  if (!output_file) {
    if (mode == 2) {
      delete[] matrix;
    }
    std::cerr << "Ошибка: не удалось открыть выходной файл\n";
    return 2;
  }

  output_file << best_column << '\n' << saddle_points_count << '\n';

  if (mode == 2) {
    delete[] matrix;
  }
  return 0;
}
