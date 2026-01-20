#include <iostream>
#include <fstream>
#include <cstring>

namespace muraviev {
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

  int getMaxSumOfDiagonals(const int* matrix, size_t side)
  {
    if (side == 0) {
      return 0;
    }

    int maxSum = 0;

    for (size_t i = 1; i < side; ++i) {
      int dgSum = matrix[i];

      for (size_t j = 1; j <= side - i - 1; ++j) {
        dgSum += matrix[j * side + j + i];
      }

      maxSum = std::max(maxSum, dgSum);
    }

    size_t count = 0;

    for (size_t i = side; i <= side * side - side; i += side) {
      int dgSum = 0;
      size_t rowIndex = 0;

      for (size_t j = 1; j <= side - count - 1; ++j) {
        dgSum += matrix[side * (j + count) + rowIndex];
        rowIndex++;
      }

      maxSum = std::max(maxSum, dgSum);
      count++;
    }

    return maxSum;
  }

  size_t fillMatrix(std::istream& fin, int* matrix, size_t rows, size_t columns)
  {
    size_t i = 0;
    for (; i < rows * columns; ++i) {
      if (!(fin >> matrix[i])) {
        break;
      }
    }
    return i;
  }

  void outToAFile(std::ostream& out, const int* matrix, size_t rows, size_t columns)
  {
    out << rows << " " << columns;
    for (size_t i = 0; i < rows * columns; ++i) {
      out << " " << matrix[i];
    }
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

  char* endptr = nullptr;
  const long num = std::strtol(argv[1], std::addressof(endptr), 10);

  if (*endptr != '\0') {
    cerr << "First parameter is not a number\n";
    return 1;
  }

  if (num != 1 && num != 2) {
    cerr << "First parameter is out of range\n";
    return 1;
  }

  const char* inputFile = argv[2];
  const char* outputFile = argv[3];
  const bool is_dynamic = (num == 2);

  size_t rows = 0;
  size_t columns = 0;

  std::ifstream fin(inputFile);

  if (!fin.is_open()) {
    cerr << "Failed to open file\n";
    return 2;
  }

  if (!(fin >> rows >> columns)) {
    cerr << "Failed to read matrix sizes\n";
    return 2;
  }

  int* matrix = nullptr;
  int fixed_matrix[10000] = {};
  size_t size = rows * columns;

  if (size > 0) {
    if (is_dynamic) {
      try {
        matrix = new int[size];
      } catch (const std::bad_alloc& err) {
        cerr << "Memory allocation failed: " << err.what() << '\n';
        return 2;
      }
    } else {
      if (size > 10000) {
        cerr << "Size of matrix is too big!\n";
        return 2;
      }
      matrix = fixed_matrix;
    }

    size_t countOfEls = muraviev::fillMatrix(fin, matrix, rows, columns);

    if (countOfEls != size) {
      cerr << "Error: Not all elements of the matrix are filled in.\n";
      if (is_dynamic) {
        delete[] matrix;
      }
      return 2;
    }
  }

  std::ofstream fout(outputFile);

  if (!fout) {
    cerr << "Output failed.\n";
    if (is_dynamic) {
      delete[] matrix;
    }
    return 2;
  }

  size_t minSide = std::min(rows, columns);
  int maxSum = muraviev::getMaxSumOfDiagonals(matrix, minSide);

  muraviev::transformMatrixSpiral(matrix, rows, columns);
  muraviev::outToAFile(fout, matrix, rows, columns);
  fout << maxSum << '\n';

  if (is_dynamic) {
    delete[] matrix;
  }

  return 0;
}
