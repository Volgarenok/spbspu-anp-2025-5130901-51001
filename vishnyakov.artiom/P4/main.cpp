#include <iostream>
namespace vishnyakov
{
  size_t Amount_of_lat_letters(char *str, size_t size); // Вариант 7
  void Uppercase_to_lowercase(char *str, size_t size); // Вариант 11
  bool is_in(char *str, char letter, size_t size);
}

bool vishnyakov::is_in(char *str, char letter, size_t size)
{
  for(size_t i=0; i<size; ++i)
  {
    if(str[i] == letter)
    {
        return 1;
    }
  }
  return 0;
}


size_t vishnyakov::Amount_of_lat_letters(char *str, size_t size)
{
    size_t result = 0;
    const char *Lowercase_lat_alphabet = "abcdefghijklmnopqrstuvwxyz";
    const char *Uppercase_lat_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for(size_t i=0; i<23;++i)
    {
      if(vishnyakov::is_in(str, Lowercase_lat_alphabet[i], size))
      {
        result += vishnyakov::is_in(str, Lowercase_lat_alphabet[i], size);
      }
      else
      {
        result += vishnyakov::is_in(str, Uppercase_lat_alphabet[i], size);
      }
    }
    return result;
}
int main()
{
}
