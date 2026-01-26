#include <iostream>
#include "functions_for_tasks.hpp"
#include "getline.hpp"

int main()
{
  using namespace vishnyakov;

  size_t size = 0;
  char **words = getWords(std::cin, size, isSpace);

  if (!words)
  {
    return 1;
  }

  for(size_t i = 0; i < size; ++i)
  {
    uppercaseToLowercase(words[i]);
    std::cout << words[i] << ' ';
    std::cout << amountOfLatLetters(words[i]) << '\n';
  }
  for (size_t i = 0; i < size; ++i)
  {
    free(words[i]);
  }
  free(words);

  return 0;
}
