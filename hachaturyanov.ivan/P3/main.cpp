#include <iostream>
#include <fstream>
#include <cstring>

int main(int argc, char** argv) {
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  } else if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }

  if (!strcmp(argv[1],"1") && !strcmp(argv[1],"2")) {
    std::cerr << "Incorrect first parameter\n";
    return 1;
  }

  std::ifstream input("input.txt");
  if (!input.is_open()) {
    std::cerr << "Cannot open the file\n";
    return 2;
  }

  int rows = 0, cols = 0;
  input >> rows >> cols;
  if (!input.good()) {
    std::cerr << "Error while reading the file\n";
    return 2;
  }

  if (!strcmp(argv[1],"1")) {
    int matrix[rows * cols];
    for (size_t i = 0; i < rows * cols; i++) {
      input >> matrix[i];
      if (!input.good()) {
        std::cerr << "Error while reading the file\n";
        return 2;
      }
    }
    input.close();
  }
}
