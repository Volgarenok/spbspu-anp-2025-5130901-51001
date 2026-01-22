#include "getline.hpp"

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
    return nullptr;
  }

  line[size] = '\0';
  return line;
}

char **vishnyakov::getWords(std::istream &input, size_t &size, bool (*isSpace)(char))
{
  size = 0;
  size_t len = 0;
  char *line = getLine(input, len);

  if (!line)
  {
    return nullptr;
  }

  if (len == 0)
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

