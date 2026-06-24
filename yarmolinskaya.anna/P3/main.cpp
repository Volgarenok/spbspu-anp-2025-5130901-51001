#include <iostream>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <memory>

namespace yarmolinskaya {
  constexpr unsigned long MAX_SIZE = 10000;

  bool readInt(std::istream& in, int& value)
  {
    long long temp = 0;
    in >> temp;
    if (!in || temp < INT_MIN || temp > INT_MAX) {
      return false;
    }
    value = static_cast<int>(temp);
    return true;
  }

  bool readMatrix(std::istream& in, int* data, unsigned long rows, unsigned long cols)
  {
    const unsigned long total = rows * cols;
    for (unsigned long i = 0; i < total; ++i)
    {
      if (!readInt(in, data[i])) {
        return false;
      }
    }
    return true;
  }

  bool isLowerTriangular(const int* m, unsigned long rows, unsigned long cols)
  {
    if (rows == 0 || cols == 0) {
      return false;
    }

    const unsigned long n = (rows < cols) ? rows : cols;
    for (unsigned long i = 0; i < n; ++i)
    {
      for (unsigned long j = i + 1; j < n; ++j)
      {
        if (m[i * cols + j] != 0) {
          return false;
        }
      }
    }
    return true;
  }

  unsigned long countColumns(const int* m, unsigned long rows, unsigned long cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }

    unsigned long result = 0;
    for (unsigned long c = 0; c < cols; ++c)
    {
      bool ok = true;
      for (unsigned long r = 1; r < rows; ++r)
      {
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

int main(int argc, char* argv[])
{
  if (argc != 4) {
    std::cerr << "Error: Invalid number of arguments\n";
    return 1;
  }

  char* end = nullptr;
  long mode = std::strtol(argv[1], std::addressof(end), 10);
  if (*end != '\0' || (mode != 1 && mode != 2)) {
    std::cerr << "Error: Invalid first argument\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input) {
    std::cerr << "Error: Cannot open input file\n";
    return 2;
  }

  unsigned long rows = 0;
  unsigned long cols = 0;
  input >> rows >> cols;
  if (!input) {
    std::cerr << "Error: Invalid matrix size\n";
    return 2;
  }

  const unsigned long total = rows * cols;
  if (mode == 1 && total > yarmolinskaya::MAX_SIZE) {
    std::cerr << "Error: Matrix too large\n";
    return 2;
  }

  int fixedBuffer[yarmolinskaya::MAX_SIZE];
  int* matrix = nullptr;

  try {
    if (mode == 1) {
      matrix = fixedBuffer;
    } else {
      matrix = new int[total];
    }

    if (!yarmolinskaya::readMatrix(input, matrix, rows, cols)) {
      throw std::runtime_error("Invalid matrix data");
    }
  } catch (...) {
    if (mode == 2) {
      delete[] matrix;
    }
    std::cerr << "Error: Invalid matrix\n";
    return 2;
  }

  std::ofstream output(argv[3]);
  if (!output) {
    if (mode == 2) {
      delete[] matrix;
    }
    std::cerr << "Error: Cannot open output file\n";
    return 2;
  }

  const bool lower = yarmolinskaya::isLowerTriangular(matrix, rows, cols);
  const unsigned long count = yarmolinskaya::countColumns(matrix, rows, cols);

  if (mode == 1) {
    output << (lower ? "true" : "false") << '\n';
  } else {
    output << count << '\n';
  }

  if (mode == 2) {
    delete[] matrix;
  }

  return 0;
}
