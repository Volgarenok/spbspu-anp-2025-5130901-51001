#include <iostream>
#include "getline.hpp"
#include "functions_for_tasks.hpp"

int main()
{
  using namespace vishnyakov;

  size_t size = 0;
  char **words = getWords(std::cin, size, isSpace);

  if (!words)
  {
    return 1;
  }

  if (size == 0)
  {
    free(words);
    return 1;
  }

  size_t total_amount_of_lat_latters = 0;
  for(size_t i = 0; i < size; ++i)
  {
    uppercaseToLowercase(words[i]);
    std::cout << words[i] << ' ';
    total_amount_of_lat_latters += amountOfLatLetters(words[i]);
  }
  std::cout << '\n' << total_amount_of_lat_latters << '\n';

  for (size_t i = 0; i < size; ++i)
  {
    free(words[i]);
  }
  free(words);

  return 0;
}
