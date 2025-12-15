#include <iostream>
namespace vishnyakov
{
  size_t Amount_of_lat_letters(char *str); // Вариант 9
  void Uppercase_to_lowercase(char *str); // Вариант 11
  bool is_in(char *str, char letter);
}
bool vishnyakov::is_in(char *str, char letter)
{
  for(size_t i=0; str[i] != '\0'; ++i)
  {
    if(str[i] == letter)
    {
        return 1;
    }
  }
  return 0;
}

size_t vishnyakov::Amount_of_lat_letters(char *str)
{
  size_t result = 0;
  const char *Lowercase_lat_alphabet = "abcdefghijklmnopqrstuvwxyz";
  const char *Uppercase_lat_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for(size_t i=0; i < 23; ++i)
  {
    if(vishnyakov::is_in(str, Lowercase_lat_alphabet[i]))
    {
      result += vishnyakov::is_in(str, Lowercase_lat_alphabet[i]);
    }
    else
    {
      result += vishnyakov::is_in(str, Uppercase_lat_alphabet[i]);
    }
  }
  return result;
}

 void vishnyakov::Uppercase_to_lowercase(char *str)
{
  for(size_t i=0; str[i] != '\0'; i++)
  {
    if (std::isalpha(str[i]))
    {
        str[i] = std::tolower(str[i]);
    }
  }
}

int main()
{
    char *str = "asdaAasG";
    std::cout << vishnyakov::Amount_of_lat_letters(str) << std::endl;
    vishnyakov::Uppercase_to_lowercase(str);
    std::cout << str;
}
