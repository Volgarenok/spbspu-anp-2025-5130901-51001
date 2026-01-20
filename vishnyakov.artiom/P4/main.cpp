#include <iostream>
namespace vishnyakov
{
  size_t amountOfLatLetters(char *str);
  void uppercaseToLowercase(char *str);
  bool isIn(char *str, char letter);
  void copy(const char *a, char *b, size_t l);
  bool isSpace(char letter);
  char *getLine(std::istream &input, size_t &size);
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
  return std::isspace(static_cast< unsigned char >(letter));
}

char *vishnyakov::getLine(std::istream &input, size_t &size)
{
  bool is_flag = input.flags() & std::ios_base::skipws;

  size_t capacity = 16;

  char *line = reinterpret_cast< char* >(malloc(sizeof(char) * capacity));

  if (!line)
  {
    return nullptr;
  }

  if (is_flag)
  {
    input >> std::noskipws;
  }

  char sym = ' ';

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
        return nullptr;
      }

      copy(line, tmp_line, size);
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

  line[size] = '\0';
  return line;
}

char **vishnyakov::getWords(std::istream &input, size_t &size, bool (*isSpace)(char))
{
  size = 0;
  size_t len = 0;
  char *line = getLine(input, len);

  if (!line || len == 0)
  {
    free(line);
    return nullptr;
  }

  size_t capacity = 10;
  char **words = reinterpret_cast< char** >(malloc(sizeof(char*) * capacity));

  if (!words)
  {
    free(line);
    return nullptr;
  }

  size_t count = 0, i = 0;
  while(i < len)
  {

    while(i < len && isSpace(line[i]))
    {
      ++i;
    }

    if (i >= len)
    {
      break;
    }

    size_t start = i;
    while (i < len && !isSpace(line[i]))
    {
      ++i;
    }

    size_t len_of_word = i - start;
    char *word = reinterpret_cast< char* >(malloc(len_of_word + 1));

    if (!word)
    {
      for (size_t j = 0; j < count; ++j)
      {
        free(words[j]);
      }
      free(line);
      free(words);
      return nullptr;
    }

    for (size_t j = 0; j < len_of_word; ++j)
    {
      word[j] = line[start + j];
    }

    word[len_of_word] = '\0';
    words[count] = word;
    count++;

    if (count == capacity)
    {
      size_t new_capacity = capacity * 2;
      char **tmp_words = reinterpret_cast< char** >(malloc(new_capacity * sizeof(char*)));

      if(!tmp_words)
      {
        for (size_t j = 0; j < count; ++j)
        {
          free(words[j]);
        }
        free(line);
        free(words);
        return nullptr;
      }

      for (size_t j = 0; j < count; ++j)
      {
        tmp_words[j] = words[j];
      }
      free(words);
      words = tmp_words;
      capacity = new_capacity;
    }
  }

  size = count;
  free(line);
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

  if (!words || size == 0)
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


