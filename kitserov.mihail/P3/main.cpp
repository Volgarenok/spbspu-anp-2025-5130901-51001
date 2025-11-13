#include <iostream>
#include <fstream>
int* create_matrix(size_t size) {
    if (size == 0) {
        return nullptr;
    }
    return new int[size];
}

void variant_1(size_t rows, size_t cols, int matrix[10000], const char* output_file)
{
	size_t answer = 0;
	bool flag;
	for (size_t i = 0; i < rows; i++)
	{
		flag = false;
		for (size_t j = 0; j < (cols-1); j++)
		{
			if (matrix[i * cols + j] == matrix[i * cols + j + 1])
			{
				flag = true;
				break;
			}
	    }
	    if (!flag)
	    {
	    	answer++;
	    }
	}
	std::ofstream output(output_file);
	output << answer;
}

void variant_2(size_t rows, size_t cols, int* matrix, const char* output_file)
{
	std::ofstream output(output_file);
	if (rows != cols or rows == 0 or cols == 0)
	{
		output << "false";
		delete[] matrix;
		return;
	}
	bool flag = true;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < i; j++) {  // только элементы ниже диагонали (j < i)
            if (matrix[i * cols + j] != 0) {
                flag = false;
                break;
            }
        }
        if (!flag) {
            break;
        }
    }
	if (flag)
	{
		output << "true";
	}
	else
	{
		output << "false";
	}
	delete[] matrix;
}

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << "Not enought arguments\n";
    return 1;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  int num = atoi(argv[1]);
  if (num != 1 and num != 2)
  {
    std::cerr << "First argument incorrect\n";
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input.is_open())
  {
    std::cerr << "Failed open input file\n";
    return 2;
  }
  size_t rows, cols;
  input >> rows >> cols;
  if (input.fail())
  {
    std::cerr << "Failed read matrix dimentions\n";
    return 2;
  }
  if (rows < 0 or cols < 0)
  {
    std::cerr << "Incorrect matrix dimensions\n";
  }
  if (num == 1)
  {
    int matrix[10000];
    for (size_t i; i < rows*cols; i++)
    {
	     input >> matrix[i];
	     if (input.fail())
	     {
	       std::cerr << "Failed read matrix's element\n";
	       return 2;
	     }
    }
    variant_1(rows, cols, matrix, argv[3]);
  }

  else
  {
    int*matrix = nullptr;
	matrix = create_matrix(rows*cols);
	if (matrix == nullptr)
	{
		std::cerr << "Failed to allocate memory for matrix\n";
		return 2;
	}
    for (size_t i; i < rows*cols; i++)
    {
	     input >> matrix[i];
	     if (input.fail())
	     {
	       std::cerr << "Failed read matrix's element\n";
	       delete[] matrix;
	       return 2;
	     }
    }
  	variant_2(rows, cols, matrix, argv[3]);
  }
  std::cout << "Succesful\n";
  return 0;
}
