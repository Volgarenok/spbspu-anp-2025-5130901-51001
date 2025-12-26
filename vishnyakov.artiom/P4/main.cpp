#include <iostream>
namespace vishnyakov
{
  size_t amountOfLatLetters(char *str);
  void uppercaseToLowercase(char *str);
  bool isIn(char *str, char letter);
  void copy(const char *a, char *b, size_t l);
  char *getline(std::istream &input, size_t &size);
}

bool vishnyakov::isIn(char *str, char letter)
{
  for(size_t i = 0; str[i] != '\0'; ++i)
  {
    if (str[i] == letter)
    {
        return 1;
    }
  }
  return 0;
}

void vishnyakov::copy(const char *a, char *b, size_t l)
{
  for (size_t i = 0; i < l; ++i)
  {
    b[i] = a[i];
  }
}

char *vishnyakov::getline(std::istream &input, size_t &size)
{
  bool is_flag = input.flags() & std::ios_base::skipws;

  size_t capacity = 16;

  char* line = reinterpret_cast<char*>(malloc(sizeof(char) * capacity));

  if (!line) 
  {
    throw std::bad_alloc();
  }

  if (is_flag) 
  {
    input >> std::noskipws;
  }

  char sym;

  while (!(input >> sym).fail() && sym != '\n')
  {
    if (size + 1 >= capacity)
    {
      size_t new_capacity = capacity * 2;
      char *tmp_line = reinterpret_cast< char* >(malloc(sizeof(char) * new_capacity));

      if (!tmp_line)
      {
        free(line);
        size = 0;
        if (is_flag)
        {
          input >> std::skipws;
        }
        throw std::bad_alloc();
      }

      vishnyakov::copy(line, tmp_line, size);
      free(line);
      capacity = new_capacity;
      line = tmp_line;
    }

    line[size++] = sym;

  }

  if (is_flag)
  {
    input >> std::skipws;
  }

  if (size == 0 && !input)
  {
    free(line);
    size = 0;
    throw std::logic_error("Input failed");
  }

  line[size] = 0;
  return line;
}

size_t vishnyakov::amountOfLatLetters(char *str)
{
  size_t result = 0;
  const char *lowercase_lat_alphabet = "abcdefghijklmnopqrstuvwxyz";
  for(size_t i = 0; i < 26; ++i)
  {
    result += vishnyakov::isIn(str, lowercase_lat_alphabet[i]);
  }
  return result;
}

 void vishnyakov::uppercaseToLowercase(char *str)
{
  for(size_t i = 0; str[i] != '\0'; i++)
  {
    if (std::isalpha(str[i]))
    {
      str[i] = std::tolower(str[i]);
    }
  }
}

int main()
{
  using namespace vishnyakov;
  size_t size = 0;
  char* line = nullptr;

  try
  {
    line = getline(std::cin, size);
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  catch (const std::logic_error& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  uppercaseToLowercase(line);
  std::cout << amountOfLatLetters(line) << '\n';
  std:: cout << line << '\n';

  free(line);
  return 0;
}

