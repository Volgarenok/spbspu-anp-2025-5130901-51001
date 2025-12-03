#include <iostream>
#include <fstream>
#include <cstring>

namespace muraviev
{
  void transformMatrixSpiral(int* matrix, size_t rows, size_t columns)
  {
    if (rows == 0 || columns == 0) {
      return;
    }

    size_t left = 0;
    size_t right = columns - 1;
    size_t top = 0;
    size_t bottom = rows - 1;

    int dec = 1;

    while (left <= right && top <= bottom) {
      for (size_t i = bottom + 1; i-- > top;) {
        matrix[i * columns + left] -= dec++;
      }
      left++;

      if (left > right) {
        break;
      }

      for (size_t j = left; j <= right; ++j) {
        matrix[top * columns + j] -= dec++;
      }
      top++;

      if (top > bottom) {
        break;
      }

      for (size_t i = top; i <= bottom; ++i) {
        matrix[i * columns + right] -= dec++;
      }
      right--;

      if (left > right) {
        break;
      }

      for (size_t j = right + 1; j-- > left;) {
        matrix[bottom * columns + j] -= dec++;
      }
      bottom--;
    }
  }

  int getMaxSumOfDiagonals(const int* matrix, size_t rows, size_t columns)
  {
    if (rows == 0 || columns == 0) {
      return 0;
    }

    int maxSum = 0;

    for (size_t i = 1; i < columns; ++i) {
      int dgSum = matrix[i];

      for (size_t j = 1; j <= columns - i - 1; ++j) {
        dgSum += matrix[j * columns + j + i];
      }

      if (maxSum < dgSum) {
        maxSum = dgSum;
      }
    }

    size_t count = 0;

    for (size_t i = columns; i <= rows * columns - columns; i += columns) {
      int dgSum = 0;
      size_t rowIndex = 0;

      for (size_t j = 1; j <= rows - count - 1; ++j) {
        dgSum += matrix[columns * (j + count) + rowIndex];
        rowIndex++;
      }

      if (maxSum < dgSum) {
        maxSum = dgSum;
      }

      count++;
    }

    return maxSum;
  }

  int readMatrixSizes(std::ifstream& fin, size_t& rows, size_t& columns)
  {
    if (!(fin >> rows >> columns)) {
      return 0;
    }
    return 1;
  }

  void fillMatrix(std::ifstream& fin, int* matrix, size_t rows, size_t columns)
  {
    for (size_t i = 0; i < rows * columns; ++i) {
      if (!(fin >> matrix[i])) {
        throw std::logic_error("func fillMatrix failed with filling elements.");
      }
    }
  }

  void outToAFile(std::ostream& out, const int* matrix, size_t rows, size_t columns)
  {
    out << rows << " " << columns;

    for (size_t i = 0; i < rows * columns; ++i) {
      out << " " << matrix[i];
    }
  }

  void outToAFile(std::ostream& out, int maxSum)
  {
    out << maxSum;
  }
}

int main(int argc, char* argv[])
{
  using std::cerr;

  if (argc < 4) {
    cerr << "Not enough arguments\n";
    return 1;
  }

  if (argc > 4) {
    cerr << "Too many arguments\n";
    return 1;
  }

  int num;

  try {
    size_t pos;
    num = std::stoi(argv[1], &pos);

    if (pos != std::strlen(argv[1])) {
      throw std::logic_error("Str in num!");
    }

    if (num != 1 && num != 2) {
      std::cerr << "First parameter is out of range\n";
      return 1;
    }
  } catch (...) {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }

  const char* inputFile = argv[2];
  const char* outputFile = argv[3];
  const bool is_dynamic = (num == 2);

  size_t rows = 0;
  size_t columns = 0;

  std::ifstream fin(inputFile);

  if (!fin.is_open()) {
    return 0;
  }

  if (!muraviev::readMatrixSizes(fin, rows, columns)) {
    std::cerr << "Failed to read matrix sizes\n";
    return 2;
  }

  int* matrix = nullptr;
  int fixed_matrix[10000];
  size_t size = rows * columns;

  if (size > 0) {
    if (is_dynamic) {
      try {
        matrix = new int[size];
      } catch (...) {
        cerr << "Error: Memory allocation failed.\n";
        return 2;
      }
    } else {
      matrix = fixed_matrix;
    }

    try {
      muraviev::fillMatrix(fin, matrix, rows, columns);
    } catch (const std::exception& err) {
      cerr << "Error: " << err.what() << std::endl;

      if (is_dynamic) {
        delete[] matrix;
      }

      return 2;
    }
  }

  fin.close();

  std::ofstream fout(outputFile);

  if (!fout) {
    std::cerr << "Output failed.\n";
    if (is_dynamic) {
      delete[] matrix;
    }
    return 2;
  }

  if (num == 1) {
    muraviev::transformMatrixSpiral(matrix, rows, columns);
    muraviev::outToAFile(fout, matrix, rows, columns);
  } else {
    if (rows != columns) {
      size_t minOne = (rows > columns) ? columns : rows;
      rows = minOne;
      columns = minOne;
    }

    int maxSum = muraviev::getMaxSumOfDiagonals(matrix, rows, columns);
    muraviev::outToAFile(fout, maxSum);
  }

  if (is_dynamic) {
    delete[] matrix;
  }

  fout.close();

  return 0;
}
