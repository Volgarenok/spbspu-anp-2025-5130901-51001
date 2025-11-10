#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

namespace alekseev
{
const size_t MAX_FIXED_SIZE = 10000;
}

int main(int argc, char* argv[])
{
  if (argc != 4) {
    std::cerr << (argc < 4 ? "Not enough arguments" : "Too many arguments") << "\n";
    return 1;
  }

  std::string numStr = argv[1];
  std::string inputFile = argv[2];
  std::string outputFile = argv[3];

  int taskNum = 0;
  try {
    taskNum = std::stoi(numStr);
  } catch (const std::exception&) {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }

  if (taskNum != 1 && taskNum != 2) {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }

  return 0;
}
