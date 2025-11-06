#include <iostream>
#include <fstream>
#include <climits>

typedef long long ll_t;

namespace stepanov {
  const size_t MAX_MATRIX_SIZE = 10000;

  void task1(ll_t (&matrix)[MAX_MATRIX_SIZE], int n, int m);
  ll_t task2(ll_t* matrix, int n, int m);
}

int main(int argc, char** argv) {
  if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }

  char* end = nullptr;
  long task_number = std::strtol(argv[1], &end, 10);
  if (*end != '\0') {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  if (task_number < 1 || task_number > 2) {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }

  std::ifstream in_file(argv[2]);
  if (!in_file.is_open()) {
    in_file.close();
    std::cerr << "Error opening input file\n";
    return 2;
  }
  int rows = 0, cols = 0;
  in_file >> rows >> cols;
  if (!in_file.good()) {
    in_file.close();
    std::cerr << "Error reading matrix sizes\n";
    return 2;
  }

  if (task_number == 1) {
    ll_t matrix[stepanov::MAX_MATRIX_SIZE];
    for (int i = 0; i < rows * cols; i++) {
      in_file >> matrix[i];
      if (!in_file.good()) {
        in_file.close();
        std::cerr << "Error reading matrix\n";
        return 2;
      }
    }
    in_file.close();

    try {
      stepanov::task1(matrix, rows, cols);
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return 2;
    }
    std::ofstream out_file(argv[3]);
    if (!out_file.is_open()) {
      std::cerr << "Error opening output file\n";
      return 2;
    }
    out_file << rows << ' ' << cols << ' ';
    for (int i = 0; i < rows * cols; i++) {
      out_file << matrix[i] << ' ';
    }
    out_file.close();
  } else if (task_number == 2) {
    ll_t* matrix = new ll_t[rows * cols];
    for (int i = 0; i < rows * cols; i++) {
      in_file >> matrix[i];
      if (!in_file.good()) {
        in_file.close();
        delete[] matrix;
        std::cerr << "Error reading matrix\n";
        return 2;
      }
    }
    in_file.close();

    ll_t result = 0;
    try {
      result = stepanov::task2(matrix, rows, cols);
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      delete[] matrix;
      return 2;
    }
    delete[] matrix;
    std::ofstream out_file(argv[3]);
    if (!out_file.is_open()) {
      std::cerr << "Error opening output file\n";
      return 2;
    }
    out_file << result;
    out_file.close();
  }
  if (in_file.is_open()) {
    in_file.close();
  }

  return 0;
}

void stepanov::task1(ll_t (&matrix)[MAX_MATRIX_SIZE], int n, int m) {
  int top = 0, bottom = n - 1, left = 0, right = m - 1;
  int i = bottom, j = left;
  int k = 1;

  while (top <= bottom && left <= right) {
    while (i >= top) {
      matrix[i * m + j] -= k++;
      i--;
    }
    i = top;
    j++;
    if (j > right) {
      break;
    }
    while (j <= right) {
      matrix[i * m + j] -= k++;
      j++;
    }
    j = right;
    i++;
    if (i > bottom) {
      break;
    }
    while (i <= bottom) {
      matrix[i * m + j] -= k++;
      i++;
    }
    i = bottom;
    j--;
    if (j < left) {
      break;
    }
    while (j > left) {
      matrix[i * m + j] -= k++;
      j--;
    }

    top++;
    bottom--;
    left++;
    right--;
    i = bottom;
    j = left;
  }
}

ll_t stepanov::task2(ll_t* matrix, int n, int m) {
  if (n * m < 2) {
    return 0;
  }
  ll_t ans = LLONG_MIN;

  for (int i = 1; i < n; ++i) {
    ll_t sum = 0;
    for (int x = i, y = 0; x < n && y < m; ++x, ++y) {
      sum += matrix[x * m + y];
    }
    ans = std::max(ans, sum);
  }

  for (int j = 1; j < m; ++j) {
    ll_t sum = 0;
    for (int x = 0, y = j; x < n && y < m; ++x, ++y) {
      sum += matrix[x * m + y];
    }
    ans = std::max(ans, sum);
  }

  return ans;
}
