#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
  std::ifstream input(argv[2]);
  int m, n;
  if(input.is_open()){
    input >> m >> n;
  }
  if (*argv[1] == '1'){
    int arrayStatic[10000];
    std:: cout << "I create static array\n";
  }
  else if (*argv[1] == '2'){
    int *arrayDinamic = new int[n*m];
    std:: cout << "I create dinamic array\n His size: " << n*m <<std::endl;
  }
  else{
    std::cout << "Error wrong num";
    return 1;
  }
  return 0;
}
