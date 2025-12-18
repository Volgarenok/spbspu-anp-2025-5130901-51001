#include <iostream>
namespace vishnyakov
{
  size_t Amount_of_lat_letters(char *str, size_t len); // Вариант 9
  void Uppercase_to_lowercase(char *str, size_t len); // Вариант 11
  bool is_in(char *str, char letter, size_t len);
  void copy(const char * a, char * b, size_t l);
  char * getline(std::istream &input, size_t &size);
}

bool vishnyakov::is_in(char *str, char letter, size_t len)
{
  for(size_t i = 0; i < len; ++i)
  {
    if(str[i] == letter)
    {
        return 1;
    }
  }
  return 0;
}

void vishnyakov::copy(const char * a, char * b, size_t l)
{
  for (size_t i = 0; i < l; ++i)
  {
    b[i] = a[i];
  }
}

char * vishnyakov::getline(std::istream &input, size_t &size)
{
  bool is_flag = input.flags() & std::ios_base::skipws;

  size_t buffer = 16;
  size_t len = 0;

  char* line = static_cast<char*>(malloc(sizeof(char) * buffer));

  if (!line) {
    size = 0;
    throw std::bad_alloc();
  }

  if (is_flag) {
    input >> std::noskipws;
  }

  char sym;

  while (!(input >> sym).fail())
  {
    if (sym == '\n') {
      break;
    }

    if (len + 1 >= buffer)
    {
      size_t new_buffer = buffer * 2;
      char* tmp_line = static_cast<char*>(malloc(sizeof(char) * new_buffer));

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

      vishnyakov::copy(line, tmp_line, len);
      free(line);
      buffer = new_buffer;
      line = tmp_line;
    }

    line[len++] = sym;

  }

  if (is_flag)
  {
    input >> std::skipws;
  }

  if (len == 0 && !input)
  {
    free(line);
    size = 0;
    throw std::logic_error("Input failed");
  }

  line[len] = 0;
  size = len;
  return line;
}

size_t vishnyakov::Amount_of_lat_letters(char *str, size_t len)
{
  size_t result = 0;
  const char *Lowercase_lat_alphabet = "abcdefghijklmnopqrstuvwxyz";
  for(size_t i = 0; i < 26; ++i)
  {
    result += vishnyakov::is_in(str, Lowercase_lat_alphabet[i], len);
  }
  return result;
}

 void vishnyakov::Uppercase_to_lowercase(char *str, size_t len)
{
  for(size_t i = 0; i < len; i++)
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

  Uppercase_to_lowercase(line, size);
  std::cout << Amount_of_lat_letters(line, size) << '\n';
  std:: cout << line << '\n';

  free(line);
  return 0;
}
