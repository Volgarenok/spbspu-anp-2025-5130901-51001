#include <iostream>
#include <fstream>
#include <cmath>

namespace shevchenko
{
  void spiralAdd(int * data, size_t rows, size_t cols);
  void averageNeighbors(const int * data, double * smooth_data, size_t rows, size_t cols);
  size_t readMatrix(std::istream & in, int * data, size_t rows, size_t cols);
  size_t writeMatrix(std::ostream & out, const int * data, size_t rows, size_t cols);
  size_t writeSmoothMatrix(std::ostream & out, const double * data, size_t rows, size_t cols);
}

void shevchenko::spiralAdd(int * data, size_t rows, size_t cols)
{
  if (rows == 0 || cols == 0)
  {
    return;
  }

  int count = 1;
  size_t top = 0;
  size_t bottom = rows - 1;
  size_t left = 0;
  size_t right = cols - 1;

  while (top <= bottom && left <= right)
  {
    for (size_t i = top; i <= bottom; ++i)
    {
      data[i * cols + left] += count;
      count++;
    }
    left++;

    if (top <= bottom)
    {
      for (size_t j = left; j <= right; ++j)
      {
        data[bottom * cols + j] += count;
        count++;
      }
      bottom--;
    }

    if (left <= right && top <= bottom)
    {
      for (size_t i = bottom; i >= top; --i)
      {
        data[i * cols + right] += count;
        count++;
        if (i == 0)
        {
          break;
        }
      }
      right--;
    }

    if (top <= bottom && left <= right)
    {
      for (size_t j = right; j >= left; --j)
      {
        data[top * cols + j] += count;
        count++;
        if (j == 0)
        {
          break;
        }
      }
      top++;
    }
  }
}

void shevchenko::averageNeighbors(const int * data, double * smooth_data, size_t rows, size_t cols)
{
  if (rows == 0 || cols == 0)
  {
    return;
  }

  for (size_t i = 0; i < rows; ++i)
  {
    for (size_t j = 0; j < cols; ++j)
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

size_t shevchenko::readMatrix(std::istream & in, int * data, size_t rows, size_t cols)
{
    if (rows == 0 || cols == 0)
    {
      return 0;
    }

    size_t total = rows * cols;
    size_t read_cnt = 0;
    for (size_t k = 0; k < total; ++k)
    {
        if (!(in >> data[k]))
        {
          return read_cnt;
        }
    }
    ++read_cnt;
    return read_cnt;
}

size_t shevchenko::writeMatrix(std::ostream & out, const int * data, size_t rows, size_t cols)
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

size_t shevchenko::writeSmoothMatrix(std::ostream & out, const double * data, size_t rows, size_t cols)
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

  int num = 0;
  try {
      num = std::stoi(argv[1]);
  } catch (...) {
      std::cerr << "First parameter is not a number";
      return 1;
  }
        
  if (num != 1 && num != 2)
  {
    std::cerr << "First parameter is out of range";
    return 1;
  }

  const char * inputFile = argv[2];
  const char * outputFile = argv[3];

  std::ifstream fin(inputFile);
  if (!fin.is_open())
  {
    std::cerr << "readMatrix failed";
    return 2;
  }

  size_t rows = 0;
  size_t cols = 0;
  if (!(fin >> rows >> cols))
  {
    std::cerr << "Cannot read matrix";
    return 2;
  }

  std::ofstream fout(outputFile);
  if (!fout.is_open())
  {
    std::cerr << "Cannot open output file";
    return 2;
  }
  if (rows == 0 && cols == 0)
  {
    fout << "0 0";
    return 0;
  }
  size_t total = rows * cols;

  if (num == 1 && total > 10000)
  {
    std::cerr << "Matrix size exceeds";
    return 2;
  }

  int * data = nullptr;
  if (num == 1)
  {
    const size_t max_size = 10000;
    static int staticData[max_size];
    data = staticData;
  }
  else
  {
    data = new int[total];
  }

  size_t read_cnt = shevchenko::readMatrix(fin, data, rows, cols);
  if (read_cnt != total)
  {
    std::cerr << "Not enough data";
    if (num == 2)
    {
      delete[] data;
    }
  return 2;
  }

  int * dataCopy = new int[total];
  for (size_t i = 0; i < total; ++i)
  {
    dataCopy[i] = data[i];
  }

  shevchenko::spiralAdd(data, rows, cols);
  shevchenko::writeMatrix(fout, data, rows, cols);
  fout << "\n";

  double * smoothData = new double[total];
  shevchenko::averageNeighbors(dataCopy, smoothData, rows, cols);
  shevchenko::writeSmoothMatrix(fout, smoothData, rows, cols);
  fout << "\n";

  delete[] dataCopy;
  delete[] smoothData;

  if (num == 2)
  {
    delete[] data;
  }
  return 0;
}
