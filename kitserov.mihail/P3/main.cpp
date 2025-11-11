#include <iostream>
#include <fstream>
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
  char* output = argv[3];
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
  int rows, cols;
  input >> rows >> cols;
  if (input.fail())
  {
    std::cerr << "Failed read matrix\n";
    return 2;
  }
}
