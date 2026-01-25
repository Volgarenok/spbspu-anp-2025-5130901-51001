#include <iostream>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <new>
namespace yarmolinskaya {
  constexpr std::size_t MAX_SIZE = 10000;

  bool readInt(std::istream& in, int& value) {
    if (!(in >> value)) {
      return false;
    }
    return true;
  }

  bool readMatrix(std::istream& in, int*& data, std::size_t& rows, std::size_t& cols, bool dynamic) {
    int r = 0, c = 0;
    if (!readInt(in, r) || !readInt(in, c)) {
      return false;
    }
    if (r < 0 || c < 0) {
      return false;
    }

    rows = static_cast<std::size_t>(r);
    cols = static_cast<std::size_t>(c);
    std::size_t total = static_cast<std::size_t>(rows * cols);

    if (!dynamic && total > MAX_SIZE) {
      return false;
    }
    if (dynamic) {
      data = new(std::nothrow) int[total];
      if (!data) {
        return false;
      }
    }

    for (std::size_t i = 0; i < total; ++i) {
      if (!readInt(in, data[i])) {
        return false;
      }
    }
    return true;
  }

  bool isLowerTriangular(const int* m, std::size_t rows, std::size_t cols) {
    if (rows == 0 || cols == 0) {
      return false;
    }
    std::size_t n = (rows < cols) ? rows : cols;
    for (std::size_t i = 0; i < rows; ++i) {
      for (std::size_t j = i + 1; j < cols; ++j) {
        if (m[i * cols + j] != 0) {
          return false;
        }
      }
    }
    return true;
  }

  std::size_t countColumns(const int* m, std::size_t rows, std::size_t cols) {
    if (rows == 0 || cols == 0) {
      return 0;
    }
    std::size_t result = 0;
    for (std::size_t c = 0; c < cols; ++c) {
      bool ok = true;
      for (std::size_t r = 1; r < rows; ++r) {
        if (m[(r - 1) * cols + c] == m[r * cols + c]) {
          ok = false;
          break;
        }
      }
      if (ok) {
        ++result;
      }
    }
    return result;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Error: Invalid number of arguments\n";
    return 1;
  }

  char* end = nullptr;
  long num = std::strtol(argv[1], &end, 10);
  if (*end != '\0' || (num != 1 && num != 2)) {
    std::cerr << "Error: Invalid first argument (must be 1 or 2)\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input) {
    std::cerr << "Error: Cannot open input file\n";
    return 2;
  }

  std::size_t rows = 0, cols = 0;
  int* matrix = nullptr;
  int staticBuffer[yarmolinskaya::MAX_SIZE];
  bool dynamic = (num == 2);

  if (!dynamic) {
    matrix = staticBuffer;
  }

  if (!yarmolinskaya::readMatrix(input, matrix, rows, cols, dynamic)) {
    std::cerr << "Error: Invalid matrix format or size\n";
    if (dynamic) {
      delete[] matrix;
    }
    return 2;
  }

  std::ofstream output(argv[3]);
  if (!output) {
    std::cerr << "Error: Cannot open output file\n";
    if (dynamic) {
      delete[] matrix;
    }
    return 2;
  }

  if (num == 1) {
    output << (yarmolinskaya::isLowerTriangular(matrix, rows, cols) ? "true" : "false") << "\n";
  } else {
    output << yarmolinskaya::countColumns(matrix, rows, cols) << "\n";
  }

  if (dynamic) {
    delete[] matrix;
  }

  return 0;
}
