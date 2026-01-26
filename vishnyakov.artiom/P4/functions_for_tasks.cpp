#include "functions_for_tasks.hpp"

size_t vishnyakov::amountOfLatLetters(char *str)
{
  size_t result = 0;
  const char *lowercase_lat_alphabet = "abcdefghijklmnopqrstuvwxyz";
  const size_t size_of_alphabet = 26;
  for (size_t i = 0; i < size_of_alphabet; ++i)
  {
    result += vishnyakov::isIn(str, lowercase_lat_alphabet[i]);
  }
  return result;
}

 void vishnyakov::uppercaseToLowercase(char *str)
{
  for (size_t i = 0; str[i] != '\0'; i++)
  {
    if (std::isalpha(str[i]))
    {
      str[i] = std::tolower(str[i]);
    }
  }
}
