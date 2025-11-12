#include <iostream>
#include <fstream>
#include <limits> 

namespace losev{
  void readFile(std::ifstream & file, int * ptrArr, size_t m, size_t n);
}

int main(int argc, char **argv){
  namespace los = losev;
  if (argc > 4){
    std::cerr << "Too many arguments\n";
    return 1;
  }
  else if(argc < 4){
    std::cerr << "Too few arguments\n";
  }
  std::ifstream input(argv[2]);
  int m = -1, n = -1;
  if(input.is_open()){
    input >> m >> n;
    if (input.fail()) {
        std::cerr << "I can not read matrix size\n";
        return 2;
    }
    if (m < 0 || n < 0){
      std::cerr << "I can not create an array from this\n";
      return 2;
    }
  }
  else{
    std::cerr <<"I can not open file\n";
    return 1;
  }
  int *arrayDinamic = nullptr;
  int arrayStack[10000];
  if (*argv[1] == '1'){
    std::cout << "I create static array\n";
    try{
      los::readFile(input, arrayStack, m, n);
    }
    catch(const std::runtime_error& e) {
        std::cerr << "Error file: " << e.what() << std::endl;
        return 2;
    }
    for(size_t i = 0; i < m; i++){
      for(size_t j = 0; j < n; j++){
        std::cout << arrayStack[i * n + j] << " ";
      }
    }
  }
  else if (*argv[1] == '2'){
    arrayDinamic = new int[n * m];
    try{
      los::readFile(input, arrayDinamic, m, n);
    }
    catch(const std::runtime_error& e) {
        std::cerr << "Error file: " << e.what() << std::endl;
        return 2;
    }
    std::cout << "I create dinamic array\n His size: " << n * m <<std::endl;
    for(size_t i = 0; i < m; i++){
      for(size_t j = 0; j < n; j++){
        std::cout << arrayDinamic[i * n + j] << " ";
      }
    }
    
  }
  else{
    std::cerr << "Error wrong num\n";
    return 1;
  }

  return 0;
}
void losev::readFile(std::ifstream & file, int * ptrArr, size_t m, size_t n){
  long long temp;
  for(size_t i = 0; i < m; i++){
    for(size_t j = 0; j < n; j++){
      file >> temp;
      if (file.fail()) {
        throw std::runtime_error("I can not read matrix");
      }
      if (temp > std::numeric_limits<int>::max()) {
        throw std::runtime_error("Number is too large for int type");
      }
      ptrArr[i * n + j] = (int)temp;
    }
  }
}