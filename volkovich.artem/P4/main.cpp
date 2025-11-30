#include <iostream>
#include <cctype>


namespace volkovich {
  void delete_pindos(char *original);
}

void volkovich::delete_pindos(char * original) {
  int position = 0;

  for (int i = 0;original[i] !='\0';i++) {
    if (std::isalpha(original[i]) && original[i] < 128) {
      continue;
    }
    original[position] = original[i];
    position++;
  }
  original[position] = '\0';
}


int main()
{
  size_t length = 0;
  char *input = new char[length + 1];
  while (!std::cin.fail())
  {
    char *tmp = new char[length + 1];
    for (int i = 0; i < length + 1; i++)
    {
      tmp[i] = input[i];
    }
    std::cin >> tmp[length];
    delete[] input;
    input = tmp;
    length++;
  }
  if (std::cin.fail() && !std::cin.eof())
  {
    std::cout << "Input failed";
    return 1;
  }
  volkovich::delete_pindos(input);
  std::cout<<'\n'<<input<<'\n';
  delete[] input;
  return 0;
}
