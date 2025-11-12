#include <iostream>
#include <fstream>
void variant_1(size_t rows, size_t cols, int matrix[][100], const char* output_file)
{
	size_t answer = 0;
	for (size_t i = 0; i < rows; i++)
	{
		bool flag = false;
		for (size_t j = 0; j < (cols-1); j++)
		{
			if (matrix[i][j] == matrix[i][j+1])
			{
				flag = true;
				answer++;
				break;
			}
	    }
	}
	std::ofstream output(output_file);
	output << answer;
	output.close();
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
    //int matrix[rows][cols];
    int matrix[100][100];
    for (size_t i; i < rows; i++)
    {
      for (size_t j; j < cols; j++)
      {
        input >> matrix[i][j];
        if (input.fail())
        {
          std::cerr << "Failed read matrix's element\n";
          return 2;
        }
      }
    }
    variant_1(rows, cols, matrix, argv[3]);
  }

  return 0;
}
