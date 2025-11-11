#include <iostream>

int main(int argc, char **argv) {
  if (argc > 1){
    std::cout << "We have " << argc - 1 <<" add param: \n";
    for(int i = 1; i < argc; i++)
    {
      std::cout << *argv[i] << std::endl;
    }
     
  }
  else{
    std::cout << "We have not add param \n";
  }
    
  return 0;
}
