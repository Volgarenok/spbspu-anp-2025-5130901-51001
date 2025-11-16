#include <iostream>
#include <fstream>
#include <cstring>

namespace muraviev
{
  void lft_bot_clk(int* matrix, size_t rows, size_t columns)
  {
    if (rows == 0 || columns == 0) {
      return;
    }

    int left = 0, right = columns - 1;
    int top = 0, bottom = rows - 1;

    int dec = 1;

    while (left <= right && top <= bottom) {
      for(int i = bottom; i >= top; --i) {
        matrix[i * columns + left] -= dec++;
      }
      left++;

      if (left > right) {
        break;
      }

      for(int j = left; j <= right; ++j) {
        matrix[top * columns + j] -= dec++;
      }
      top++;

      if (top > bottom) {
        break;
      }

      for(int i = top; i <= bottom; ++i) {
        matrix[i * columns + right] -= dec++;
      }
      right--;

      if (left > right) {
        break;
      }

      for(int j = right; j >= left; --j) {
        matrix[bottom * columns + j] -= dec++;
      }
      bottom--;
    }
  }

  int max_sum_sdg(const int* matrix, size_t rows, size_t columns)
  {
    if (rows == 0 || columns == 0) {
      return 0;
    }

    int maxSum = 0;

    for(size_t i = 1; i < columns; ++i) {
      int dgSum = matrix[i];

      for(size_t j = 1; j <= columns - i - 1; ++j) {
        dgSum += matrix[j * columns + j + i];
      }

      if (maxSum < dgSum) {
        maxSum = dgSum;
      }
    }

    size_t count = 0;

    for(size_t i = columns; i <= rows * columns - columns; i += columns) {
      int dgSum = 0;
      size_t rowIndex = 0;

      for(size_t j = 1; j <= rows - count - 1; ++j) {
        dgSum += matrix[columns * (j + count) + rowIndex];
        rowIndex++;
      }

      count++;

      if (maxSum < dgSum) {
        maxSum = dgSum;
      }
    }

    return maxSum;
  }

  int readMatrixSizes(std::ifstream& fin, size_t& rows, size_t& columns)
  {
    if (!fin) {
        std::cerr << "ReadMatrixSizes Error: Reading failed.\n";
        return 0;
    }

    if (!(fin >> rows >> columns)) {
        std::cerr << "ReadMatrixSizes Error: Reading sizes of matrix failed.\n";
        return 0;
    }

    if (rows <= 0 || columns <= 0) {
        std::cerr << "ReadMatrixSizes Error: Sizes must be positive.\n";
        return 0;
    }

    return 1;
  }

  void fillMatrix(std::ifstream& fin, int * matrix, size_t rows, size_t columns)
  {
    for (int i = 0; i < rows * columns; ++i) {
      if (!(fin >> matrix[i])) {
        throw std::logic_error("func fillMatrix failed with filling elements.");
      }
    }
  }

  void outToAFile(std::ostream& out, const int * matrix, size_t rows, size_t columns)
  {
    out << rows << " " << columns;

    for(size_t i = 0; i < rows * columns; ++i) {
      out << " " << matrix[i];
    }
  }

  void outToAFile(std::ostream& out, int maxSum)
  {
    out << maxSum;
  }
}

int main(int argc, char* argv[]) {
  using std::cerr;

  if (argc < 4) {
    cerr << "Not enough arguments";
    return 1;
  }

  if (argc > 4) {
    cerr << "Too many arguments";
    return 1;
  }

  int num;

  if (strcmp(argv[1], "1") || strcmp(argv[1], "2")) {
    num = std::stoi(argv[1]);
  } else {
    cerr << "First parameter is not a number";
    return 1;
  }

  if (num != 1 && num != 2) {
    std::cerr << "First parameter is out of range";
    return 1;
  }

  const char* inputFile = argv[2];
  const char* outputFile = argv[3];
  size_t rows = 0;
  size_t columns = 0;
  bool is_dynamic = (num == 2);

  std::ifstream fin(inputFile);

  if (!muraviev::readMatrixSizes(fin, rows, columns)) {
    cerr << "Reading matrix sizes failed.\n";
    return 2;
  }

  int* matrix = nullptr;
  int static_matrix[10000];
  size_t size = rows * columns;

  if (size > 0) {
    if (is_dynamic) {
      matrix = new int[size];
    } else {
      matrix = static_matrix;
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
    std::cerr << "Output failed.";
    if (is_dynamic) {
      delete[] matrix;
    }
    return 2;
  }

  if (num == 1) {
    muraviev::lft_bot_clk(matrix, rows, columns);
    muraviev::outToAFile(fout, matrix, rows, columns);
  } else {
    if (rows != columns) {
      size_t minOne = rows > columns ? columns : rows;
      rows = minOne;
      columns = minOne;
    }

    int maxSum = muraviev::max_sum_sdg(matrix, rows, columns);
    muraviev::outToAFile(fout, maxSum);
  }

  if (is_dynamic) {
    delete[] matrix;
  }
  fout.close();

  return 0;
}
