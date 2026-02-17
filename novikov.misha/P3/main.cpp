#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

namespace novikov {

  void lftTopCnt(int * matrix, int rows, int cols)
  {
    int step = 1;
    int top = 0;
    int bottom = rows - 1;
    int left = 0;
    int right = cols - 1;

    while (top <= bottom && left <= right) {
      for (int r = top; r <= bottom; ++r) {
        matrix[r * cols + left] += step++;
      }
      ++left;

      if (left > right) {
        break;
      }

      for (int c = left; c <= right; ++c) {
        matrix[bottom * cols + c] += step++;
      }

      if (top == bottom) {
        break;
      }
      --bottom;

      for (int r = bottom; r >= top; --r) {
        matrix[r * cols + right] += step++;
      }
      --right;

      for (int c = right; c >= left; --c) {
        matrix[top * cols + c] += step++;
      }
      ++top;
    }
  }

  void buildSmoothed(const int * src, double * dst, int rows, int cols)
  {
    const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int r = 0; r < rows; ++r) {
      for (int c = 0; c < cols; ++c) {
        double sum = 0.0;
        int count = 0;
        for (int k = 0; k < 8; ++k) {
          const int nr = r + dr[k];
          const int nc = c + dc[k];
          if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
            sum += src[nr * cols + nc];
            ++count;
          }
        }
        dst[r * cols + c] = (count > 0) ? sum / count : 0.0;
      }
    }
  }

  size_t fillMatrix(std::istream & in, int * matrix, size_t n)
  {
    size_t i = 0;
    for (; i < n; ++i) {
      if (!(in >> matrix[i])) {
        break;
      }
    }
    return i;
  }

  void writeMatrix(std::ostream & out, const int * matrix, size_t rows, size_t cols)
  {
    out << rows << ' ' << cols;
    for (size_t i = 0; i < rows * cols; ++i) {
      out << ' ' << matrix[i];
    }
    out << '\n';
  }

  void writeSmoothed(std::ostream & out, const double * data, size_t rows, size_t cols)
  {
    out << rows << ' ' << cols << std::fixed << std::setprecision(1);
    for (size_t i = 0; i < rows * cols; ++i) {
      out << ' ' << data[i];
    }
    out << '\n';
  }

}

int main(int argc, char ** argv)
{
  if (argc != 4) {
    std::cerr << "Wrong number of arguments\n";
    return 1;
  }

  char * endptr = nullptr;
  const long num = std::strtol(argv[1], &endptr, 10);

  if (*endptr != '\0') {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }

  if (num != 1 && num != 2) {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }

  std::ifstream fin(argv[2]);
  if (!fin) {
    std::cerr << "Failed to open input file\n";
    return 2;
  }

  size_t rows = 0;
  size_t cols = 0;

  if (!(fin >> rows >> cols)) {
    std::cerr << "Failed to read matrix dimensions\n";
    return 2;
  }

  const size_t n = rows * cols;

  int * matrix = nullptr;
  int * orig = nullptr;
  double * smooth = nullptr;

  int fixed_matrix[10000];
  int fixed_orig[10000];
  double fixed_smooth[10000];

  if (num == 1) {
    if (n > 10000) {
      std::cerr << "Matrix is too large for static storage\n";
      return 2;
    }
    matrix = fixed_matrix;
    orig = fixed_orig;
    smooth = fixed_smooth;
  } else {
    matrix = new (std::nothrow) int[n];
    orig = new (std::nothrow) int[n];
    smooth = new (std::nothrow) double[n];
    if (!matrix || !orig || !smooth) {
      std::cerr << "Memory allocation failed\n";
      delete[] matrix;
      delete[] orig;
      delete[] smooth;
      return 2;
    }
  }

  if (n > 0 && novikov::fillMatrix(fin, matrix, n) != n) {
    std::cerr << "Not all matrix elements could be read\n";
    if (num == 2) {
      delete[] matrix;
      delete[] orig;
      delete[] smooth;
    }
    return 2;
  }

  std::ofstream fout(argv[3]);
  if (!fout) {
    std::cerr << "Failed to open output file\n";
    if (num == 2) {
      delete[] matrix;
      delete[] orig;
      delete[] smooth;
    }
    return 2;
  }

  if (n > 0) {
    std::memcpy(orig, matrix, n * sizeof(int));
    novikov::lftTopCnt(matrix, static_cast< int >(rows), static_cast< int >(cols));
    novikov::buildSmoothed(orig, smooth, static_cast< int >(rows), static_cast< int >(cols));
  }

  novikov::writeMatrix(fout, matrix, rows, cols);
  novikov::writeSmoothed(fout, smooth, rows, cols);

  if (num == 2) {
    delete[] matrix;
    delete[] orig;
    delete[] smooth;
  }

  return 0;
}
