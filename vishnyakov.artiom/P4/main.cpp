#include <iostream>
namespace vishnyakov
{
  size_t amountOfLatLetters(char *str);
  void uppercaseToLowercase(char *str);
  bool isIn(char *str, char letter);
  void copy(const char *a, char *b, size_t l);
  bool isSpace(char letter);
  char **getWords(std::istream &input, size_t &size, bool (*isSpace)(char));
}

bool vishnyakov::isIn(char *str, char letter)
{
  for (size_t i = 0; str[i] != '\0'; ++i)
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

bool vishnyakov::isSpace(char letter)
{
  return std::isspace(static_cast<unsigned char>(letter));
}

char **vishnyakov::getWords(std::istream &input, size_t &size, bool (*isSpace)(char))
{
  bool is_flag = input.flags() & std::ios_base::skipws;

  size_t capacity = 16;
  size_t len = 0;

  char *line = reinterpret_cast<char *>(malloc(sizeof(char) * capacity));

  if (!line)
  {
    return nullptr;
  }

  size_t capacity_for_words = 10;
  char **words = reinterpret_cast<char **>(malloc(sizeof(char *) * capacity_for_words));

  if (!words)
  {
    free(line);
    return nullptr;
  }

  if (is_flag)
  {
    input >> std::noskipws;
  }

  char sym = ' ';

  while (!(input >> sym).fail())
  {
    if (len + 1 >= capacity)
    {
      size_t new_capacity = capacity * 2;
      char *tmp_line = reinterpret_cast<char *>(malloc(sizeof(char) * new_capacity));

      if (!tmp_line)
      {
        free(line);
        free(words);
        size = 0;

        if (is_flag)
        {
          input >> std::skipws;
        }
        return nullptr;
      }

      copy(line, tmp_line, len);
      free(line);
      capacity = new_capacity;
      line = tmp_line;
    }

    if (isSpace(sym) && len != 0)
    {
      char *word = reinterpret_cast<char *>(malloc(len + 1));

      if (!word)
      {
        for (size_t j = 0; j < size; ++j)
        {
          free(words[j]);
        }

        if (is_flag)
        {
          input >> std::skipws;
        }

        free(line);
        free(words);

        size = 0;

        return nullptr;
      }

      for (size_t i = 0; i < len; ++i)
      {
        word[i] = line[i];
      }

      word[len] = '\0';
      words[size++] = word;
      len = 0;

      if (size == capacity_for_words)
      {
        size_t new_capacity_for_words = capacity_for_words * 2;
        char **tmp_words = reinterpret_cast<char **>(malloc(new_capacity_for_words * sizeof(char *)));

        if (!tmp_words)
        {
          for (size_t j = 0; j < size; ++j)
          {
            free(words[j]);
          }

          if (is_flag)
          {
            input >> std::skipws;
          }

          free(line);
          free(words);

          size = 0;

          return nullptr;
        }

        for (size_t j = 0; j < size; ++j)
        {
          tmp_words[j] = words[j];
        }

        free(words);
        words = tmp_words;
        capacity_for_words = new_capacity_for_words;
        capacity = 8;
      }

      if (sym == '\n')
      {
        free(line);
        break;
      }
    }

    if (!isSpace(sym))
    {
      line[len++] = sym;
    }
  }

  if (is_flag)
  {
    input >> std::skipws;
  }

  if (size == 0 && !input)
  {
    free(line);
    free(words);

    size = 0;

    return nullptr;
  }

  return words;
}

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

int main()
{
  using namespace vishnyakov;

  size_t size = 0;
  char **words = getWords(std::cin, size, isSpace);

  if (!words)
  {
    return 1;
  }

  for (size_t i = 0; i < size; ++i)
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
