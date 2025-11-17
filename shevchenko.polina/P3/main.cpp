#include <iostream>
#include <fstream>
#include <cmath>

namespace shevchenko
{
  void lft_top_cnt(int* data, int rows, int cols);
  void bld_smt_mtr(const int * data, double * smooth_data, size_t rows, size_t cols);
  size_t read_matrix(std::istream & in, int * data, size_t rows, size_t cols);
  size_t write_matrix(std::ostream & out, const int * data, size_t rows, size_t cols);
  size_t write_smooth_matrix(std::ostream & out, const double * data, size_t rows, size_t cols);
}

void shevchenko::lft_top_cnt(int * data, int rows, int cols)
{
  if (rows == 0 || cols == 0)
  {
    return;
  }

  int count = 1;
  int top = 0, bottom = rows - 1;
  int left = 0, right = cols - 1;

  while (top <= bottom && left <= right)
  {
    for (int i = top; i <= bottom; ++i)
    {
      data[i * cols + left] += count;
      count++;
    }
    left++;

    if (top <= bottom)
    {
      for (int j = left; j <= right; ++j)
      {
        data[bottom * cols + j] += count;
        count++;
      }
      bottom--;
    }

    if (left <= right && top <= bottom)
    {
      for (int i = bottom; i >= top; --i)
      {
        data[i * cols + right] += count;
        count++;
      }
      right--;
    }

    if (top <= bottom && left <= right)
    {
      for (int j = right; j >= left; --j)
      {
        data[top * cols + j] += count;
        count++;
      }
      top++;
    }
  }
}

void shevchenko::bld_smt_mtr(const int * data, double * smooth_data, size_t rows, size_t cols)
{
  if (rows == 0 || cols == 0)
  {
    return;
  }

  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      double sum = 0.0;
      int count = 0;

      if (i > 0 && j > 0)
      {
        sum += data[(i - 1) * cols + (j - 1)];
        count++;
      }

      if (i > 0)
      {
        sum += data[(i - 1) * cols + j];
        count++;
      }

      if (i > 0 && j < cols - 1)
      {
        sum += data[(i - 1) * cols + (j + 1)];
        count++;
      }

      if (j > 0)
      {
        sum += data[i * cols + (j - 1)];
        count++;
      }

      if (j < cols - 1)
      {
        sum += data[i * cols + (j + 1)];
        count++;
      }

      if (i < rows - 1 && j > 0)
      {
        sum += data[(i + 1) * cols + (j - 1)];
        count++;
      }

      if (i < rows - 1)
      {
        sum += data[(i + 1) * cols + j];
        count++;
      }

      if (i < rows - 1 && j < cols - 1)
      {
        sum += data[(i + 1) * cols + (j + 1)];
        count++;
      }

      if (count > 0)
      {
        smooth_data[i * cols + j] = std::round((sum / count) * 10) / 10.0;
      }
      else
      {
        smooth_data[i * cols + j] = 0.0;
      }
    }
  }
}

size_t shevchenko::read_matrix(std::istream & in, int * data, size_t rows, size_t cols)
{
  if (rows == 0 || cols == 0)
  {
    return 0;
  }

  size_t total = rows * cols;
  for (size_t k = 0; k < total; ++k)
  {
    if (!(in >> data[k]))
    {
      throw std::logic_error("read_matrix failed: not enough data");
    }
  }
    return total;
}

size_t shevchenko::write_matrix(std::ostream & out, const int * data, size_t rows, size_t cols)
{
  out << rows << " " << cols;
  for (size_t r = 0; r < rows; ++r)
  {
    for (size_t c = 0; c < cols; ++c)
    {
      out << " " << data[r * cols + c];
    }
  }
  return rows * cols;
}

size_t shevchenko::write_smooth_matrix(std::ostream & out, const double * data, size_t rows, size_t cols)
{
  out << rows << " " << cols;
  for (size_t r = 0; r < rows; ++r)
  {
    for (size_t c = 0; c < cols; ++c)
    {
      out << " " << data[r * cols + c];
    }
  }
  return rows * cols;
}


int main(int argc, char * argv[])
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments";
    return 1;
  }

  if (argc > 4)
  {
    std::cerr << "Too many arguments";
    return 1;
  }

  int num;

  try {
    num = std::stoi(argv[1]);
  } catch (...) {
    std::cerr << "First parameter is not a number";
    return 1;
  }

  if (num != 1 && num != 2) {
    std::cerr << "First parameter is out of range";
    return 1;
  }

  const char * inputFile = argv[2];
  const char * outputFile = argv[3];

  std::ifstream fin(inputFile);
  if (!fin.is_open())
  {
    std::cerr << "read_matrix failed";
    return 2;
  }

  int rows = 0;
  int cols = 0;
  if (!(fin >> rows >> cols))
  {
    std::cerr << "read_matrix failed";
    return 2;
  }

  size_t total = rows * cols;
  int * data = nullptr;
  if (total > 0)
  {
    try {
      data = new int[total];
    } catch (...) {
      std::cerr << "read_matrix failed";
      return 2;
    }

    try {
      shevchenko::read_matrix(fin, data, rows, cols);
    } catch (const std::exception& e) {
      std::cerr << e.what();
      delete[] data;
      return 2;
    }
  }

  std::ofstream fout(outputFile);
  if (!fout.is_open())
  {
    std::cerr << "read_matrix failed";
    delete[] data;
    return 2;
  }

  if (num == 1)
  {
    shevchenko::lft_top_cnt(data, rows, cols);
    shevchenko::write_matrix(fout, data, rows, cols);
  } else
  {
    double * smooth_data = new double[total];
    shevchenko::bld_smt_mtr(data, smooth_data, rows, cols);
    shevchenko::write_smooth_matrix(fout, smooth_data, rows, cols);
    delete[] smooth_data;
  }

  delete[] data;
  return 0;
}
