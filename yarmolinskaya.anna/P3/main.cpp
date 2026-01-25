#include <iostream>
#include <fstream>
#include <cstdlib>

const int kMaxElements = 10000;

namespace yarmolinskaya
{
  bool parseInt(const char* s_ptr, int& value)
  {
    if (s_ptr == nullptr || *s_ptr == '\0')
    {
      return false;
    }

    char* end_ptr = nullptr;
    long v = std::strtol(s_ptr, &end_ptr, 10);

    if (*end_ptr != '\0')
    {
      return false;
    }

    value = static_cast<int>(v);
    return true;
  }

  int countColumns(const int* matrix_ptr, int rows, int cols)
  {
    int k_size = (rows < cols) ? rows : cols;
    int result_count = 0;

    for (int j = 0; j < k_size; ++j)
    {
      bool is_ok = true;
      for (int i = 0; i + 1 < k_size; ++i)
      {
        if (matrix_ptr[i * cols + j] == matrix_ptr[(i + 1) * cols + j])
        {
          is_ok = false;
          break;
        }
      }
      if (is_ok)
      {
        ++result_count;
      }
    }

    return result_count;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    std::cerr << "Invalid number of arguments, expected 3 arguments but got " << argc - 1 << "\n";
    return 1;
  }

  int task_num = 0:
  if (!yarmolinskaya::parseInt(argv[1], task_num) || (task_num != 1 && task_num != 2))
  {
    std::cerr << "Invalid task number\n";
    return 1;
  }

  std::ifstream input_file(argv[2]);
  if (!input_file)
  {
    std::cerr << "Cannot open input file\n";
    return 2;
  }

  std::ofstream output_file(argv[3]);
  if (!output_file)
  {
    std::cerr << "Cannot open output file\n";
    return 2;
  }

  int rows_count = 0, cols_count = 0;
  if (!(input_file >> rows_count >> cols_count) || rows_count < 0 || cols_count < 0)
  {
    std::cerr << "Invalid matrix format\n";
    return 2;
  }

  int total_elements = rows_count * cols_count;
  if (task_num == 1 && total_elements > kMaxElements)
  {
    std::cerr << "Matrix is too large\n";
    return 2;
  }

  int* matrix_ptr = nullptr;
  int fixed_matrix[kMaxElements];

  if (task_num == 1)
  {
    matrix_ptr = fixed_matrix;
  }
  else
  {
    matrix_ptr = new int[total_elements];
  }

  for (int i = 0; i < total_elements; ++i)
  {
    if (!(input_file >> matrix_ptr[i]))
    {
      std::cerr << "Invalid matrix data\n";
      if (task_num == 2)
      {
        delete[] matrix_ptr;
      }
      return 2;
    }
  }

  int extra_data = 0;
  if (input_file >> extra_data)
  {
    std::cerr << "Extra data in input file\n";
    if (task_num == 2)
    {
      delete[] matrix_ptr;
    }
    return 2;
  }

  int result_value = yarmolinskaya::countColumns(matrix_ptr, rows_count, cols_count);

  output_file << result_value;

  if (task_num == 2)
  {
    delete[] matrix_ptr;
  }

  return 0;
}
