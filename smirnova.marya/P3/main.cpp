#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cmath>
namespace smirnova {
  struct Dynamic {
    int rows, cols;
    int* matrix = nullptr;
    void freed() { delete[] matrix; matrix = nullptr; }
    int localmaximum() const;
    int elementsdiagonal() const;
  };
  struct Static {
    int rows, cols;
    int matrix[1000] = {};
    int localmaximum() const;
    int elementsdiagonal() const;
  };
}
int smirnova::Static::localmaximum() const {
  int count = 0;
  for (int i = 1; i < rows - 1; ++i) {
    for (int j = 1; j < cols - 1; ++j) {
      int val = matrix[i * cols + j];
      if (matrix[i * cols + j - 1] < val &&
        matrix[i * cols + j + 1] < val &&
        matrix[(i - 1) * cols + j] < val &&
        matrix[(i + 1) * cols + j] < val &&
        matrix[(i - 1) * cols + j - 1] < val &&
        matrix[(i + 1) * cols + j + 1] < val &&
        matrix[(i + 1) * cols + j - 1] < val &&
        matrix[(i - 1) * cols + j + 1] < val) {
        ++count;
      }
    }
  }
  return count;
}
int smirnova::Dynamic::localmaximum() const {
  int count = 0;
  for (int i = 1; i < rows - 1; ++i) {
    for (int j = 1; j < cols - 1; ++j) {
      int val = matrix[i * cols + j];
      if (matrix[i * cols + j - 1] < val &&
        matrix[i * cols + j + 1] < val &&
        matrix[(i - 1) * cols + j] < val &&
        matrix[(i + 1) * cols + j] < val &&
        matrix[(i - 1) * cols + j - 1] < val &&
        matrix[(i + 1) * cols + j + 1] < val &&
        matrix[(i + 1) * cols + j - 1] < val &&
        matrix[(i - 1) * cols + j + 1] < val) {
        ++count;
      }
    }
  }
  return count;
}
int smirnova::Static::elementsdiagonal() const {
  int n = (rows < cols ? rows : cols);
  int maxsum = 0;
  for (int sum = 0; sum <= 2*(n-1); ++sum) {
    if (sum == n-1) continue;
      int diagsum = 0;
      for (int i = 0; i < n; ++i) {
        int j = sum - i;
        if (j >= 0 && j < n) {
          diagsum += matrix[i * cols + j];
        }
      }
      if (diagsum > maxsum) maxsum = diagsum;
  }
  return maxsum;
}
int smirnova::Dynamic::elementsdiagonal() const {
  int n = (rows < cols ? rows : cols);
  int maxsum = 0;
  for (int sum = 0; sum <= 2*(n-1); ++sum) {
    if (sum == n-1) continue;
    int diagsum = 0;
    for (int i = 0; i < n; ++i) {
      int j = sum - i;
      if (j >= 0 && j < n) {
        diagsum += matrix[i * cols + j];
      }
    }
    if (diagsum > maxsum) maxsum = diagsum;
  }
  return maxsum;
}
int main(int argc, char** argv) {
  if (argc != 4) {
    std::cerr << "Incorrect number of arguments\n";
    return 1;
  }
  if (strcmp(argv[1], "1") != 0 && strcmp(argv[1], "2") != 0) {
    std::cerr << "Garbage in argument 1\n";
    return 1;
  }
  else if (!std::stoi(argv[1])) {
    std::ifstream input(argv[2]);
    if (!input.is_open()) {
      std::cerr << "Cannot open input file\n";
      return 1;
    }
    smirnova::Static s;
    if ((!(input >> s.rows >> s.cols) || s.rows < 0 || s.cols < 0) || s.rows * s.cols > 1000) {
      std::cerr << "Error: invalid matrix size\n";
      return 2;
    }
    else if (s.rows == 0 || s.cols == 0) {
      std::ofstream output(argv[3]);
      output << "0 0\n0\n";
      return 0;
    }
    else {
      for (int i = 0; i < s.rows * s.cols; ++i) {
        if (!(input >> s.matrix[i]) || std::abs(s.matrix[i]) > 1000000) {
          std::cerr << "Error: invalid element or out of range\n";
          return 2;
        }
      }
      char extra;
      if (input >> extra) {
        std::cerr << "Error: extra data in input file (e.g., letter '" << extra << "')\n";
        return 2;
      }
      input.close();
      std::ofstream output(argv[3]);
      if (s.rows >=3 && s.cols >= 3) output << s.localmaximum() << "\n";
      else output << "0\n";
      output << s.elementsdiagonal() << "\n";
      output.close();
    }
  }
  else {
    std::ifstream input(argv[2]);
    if (!input.is_open()) {
      std::cerr << "Cannot open input file\n";
      return 1;
    }
    smirnova::Dynamic d;
    if (!(input >> d.rows >> d.cols) || d.rows < 0 || d.cols < 0 || d.rows * d.cols > 1000000) {
      std::cerr << "Error: invalid matrix size\n";
      return 2;
    }
    else if (d.rows == 0 || d.cols == 0) {
      std::ofstream output(argv[3]);
      output << "0 0\n0\n";
      return 0;
    }
    else {
      d.matrix = new int[d.rows * d.cols];
      for (int i = 0; i < d.rows * d.cols; ++i) {
        if (!(input >> d.matrix[i]) || std::abs(d.matrix[i]) > 1000000) {
          std::cerr << "Error: invalid element or out of range\n";
          d.freed();
          return 2;
        }
      }
      char extra;
      if (input >> extra) {
        std::cerr << "Error: extra data in input file (e.g., letter '" << extra << "')\n";
        d.freed();
        return 2;
      }
      input.close();
      std::ofstream output(argv[3]);
      if (d.rows >=3 && d.cols >= 3) output << d.localmaximum() << "\n";
      else output << "0\n";
      output << d.elementsdiagonal() << "\n";
      output.close();
      d.freed();
    }
  }
  return 0;
}
