#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>


namespace borisov
{
  int count_local_maximum(const int* matrix, int rows, int cols)
  {
    int count = 0;
      for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
          int idx = i * cols + j;
          int current = matrix[idx];
          bool is_max = true;
          for (int di = -1; di <= 1 && is_max; di++) {
            for (int dj = -1; dj <= 1 && is_max; dj++) {
              if (di == 0 && dj == 0) {
                continue;
              }
              int neighbor_idx = (i + di) * cols + (j + dj);
              if (neighbor_idx >= 0) {
                if (current <= matrix[neighbor_idx]) {
                  is_max = false;
                }
              } 
            }
          }
          if (is_max) {
            count++;
          }
        }
      }
    return count;
  }

  int get_max_sum_antidiagonal(const int* matrix, int rows, int cols)
  {
    int n;
    if (rows < cols){
      n = rows;
    } else {
      n = cols;
    }
    int max_diagsum = 0;
    for (int sum = 0; sum <= 2*(n-1); ++sum){
      if (sum == n-1){
        continue;
      }
      int diagsum = 0;
      for (int i = 0; i < n; ++i){
        int j = sum - i;
        if ((j >= 0) && (j < n)){
          diagsum += matrix[i * cols + j];
        }
      }
      if (diagsum > max_diagsum){
        max_diagsum = diagsum;
      }
    }
    return max_diagsum;
  }

  void process_output(std::ostream& output, int rows, int cols, const int* matrix)
  {
    if (rows >=3 && cols >= 3){
      output << borisov::count_local_maximum(matrix, rows, cols) << "\n";
    } else {
      output << "0\n";
    } 
    output << borisov::get_max_sum_antidiagonal(matrix, rows, cols) << "\n";
  } 
}


int main(int argc, char ** argv)
{
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  } else if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  
  if ((*argv[1] != '1') && (*argv[1] != '2')) {
    std::cerr << "First argument is out of range or not a number\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cerr << "Read matrix failed";
    return 2;
  }
  int rows = 0, cols = 0;

  if (!(input >> rows >> cols) || rows < 0 || cols < 0 || rows * cols > 10000) {
    std::cerr << "readMatrix failed";
    return 2;
  }

  if (rows == 0 || cols == 0) {
    std::ofstream output(argv[3]);
    output << "0 0\n0\n";
    return 0;
  }

  if ((*argv[1] == '1')) {
    int matrix[10000] = {};
    for (int i = 0; i < rows * cols; ++i) {
      if (!(input >> matrix[i]) || std::abs(matrix[i]) > 10000000) {
        std::cerr << "Error: invalid element or out of range\n";
        return 2;
      }
    }
    input.close();
    std::ofstream output(argv[3]);
    borisov::process_output(output, rows, cols, matrix);
    output.close();
  } else if ((*argv[1] == '2')) {
    int* matrix = new int[rows * cols];
    for (int i = 0; i < rows * cols; ++i) {
      if (!(input >> matrix[i]) || std::abs(matrix[i]) > 10000000) {
        std::cerr << "Error: invalid element or out of range\n";
        delete[] matrix;
        return 2;
      }
    }
    input.close();
    std::ofstream output(argv[3]);
    borisov::process_output(output, rows, cols, matrix);
    output.close();
    delete[] matrix;
  }
  return 0;
}
