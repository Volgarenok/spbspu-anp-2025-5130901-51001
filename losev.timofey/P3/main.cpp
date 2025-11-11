#include <iostream>

int main(int argc, char **argv) {
  int n = 2, m = 2;
  if (*argv[1] == '1'){
    int arrayStatic[10000];
    std:: cout << "I create static array\n";
  }
  else if (*argv[1] == '2'){
    int *arrayDinamic = new int[n*m];
    std:: cout << "I create dinamic array\n";
  }
}
