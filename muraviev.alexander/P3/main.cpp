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

  int max_sum_sdg(const int* data, size_t rows, size_t columns) 
  {
    if (rows == 0 || columns == 0) {
      return 0;
    }

    int maxSum = 0;
    
    for(size_t i = 1; i < columns; ++i) {
      int dgSum = data[i];

      for(size_t j = 1; j <= columns - i - 1; ++j) {
        dgSum += data[j * columns + j + i];
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
        dgSum += data[columns * (j + count) + rowIndex];
        rowIndex++;
      }

      count++;

      if (maxSum < dgSum) {
        maxSum = dgSum;
      }
    }

    return maxSum;
  }
}
           

int main(int argc, char* argv[]) {
  using std::cout, std::endl;
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

  try {
    int num = std::stoi(argv[1]);
    cout << num;;
  } catch (...) {
    cerr << "First parameter is not a number";
    return 1;
  }

  if (num != 1 && num != 2) {
    cerr << "First parameter is out of range";
    return 1;
  }
}
