#include <iostream>
#include <cctype>

namespace volkovich
{
  void deletePindos(char *original);
  void addMissing(char *original, char *buffer);
  size_t getAddMissingSize(char *original);
}

void volkovich::deletePindos(char *original)
{
  size_t position = 0;

  for (size_t i = 0; original[i] != '\0'; i++)
  {
    if (std::isalpha(original[i]) && (64 < original[i] < 91 || 96 < original[i] < 123))
    {
      continue;
    }
    original[position] = original[i];
    position++;
  }
  original[position] = '\0';
}

void volkovich::addMissing(char *original, char *buffer)
{
  size_t last_symb = 0;
  for (size_t i = 0; original[i] != '\0'; i++)
  {
    last_symb++;
  }
  for (size_t asc = 97; asc < 123; asc++)
  {
    bool contained = false;
    for (size_t i = 0; original[i] != '\0'; i++)
    {
      if (tolower(original[i]) == char(asc))
      {
        contained = true;
        break;
      }
    }
    if (!contained)
    {
      buffer[last_symb] = char(asc);
      last_symb++;
    }
  }
}

size_t volkovich::getAddMissingSize(char *original)
{
  size_t ouput_len = 0;
  for (size_t i = 0; original[i] != '\0'; i++)
  {
    ouput_len++;
  }
  for (size_t asc = 97; asc < 123; asc++)
  {
    bool contained = false;
    for (size_t i = 0; original[i] != '\0'; i++)
    {
      if (tolower(original[i]) == ((char)asc))
      {
        contained = true;
        break;
      }
    }
    if (!contained)
    {
      ouput_len++;
    }
  }
  return ouput_len;
}
int main()
{
  size_t length = 0;
  char *input = nullptr;
  char *tmp= nullptr;
  try
  {
    input = new char[length + 1];
  }
  catch (...)
  {
    delete[] input;
    return 1;
  }
  while (!std::cin.fail())
  {
    try
    {
      tmp = new char[length + 1];
    }
    catch (...)
    {
      delete[] tmp;
      delete[] input;
      return 1;
    }
    for (size_t i = 0; i < length + 1; i++)
    {
      tmp[i] = input[i];
    }
    std::cin >> tmp[length];
    delete[] input;
    input = tmp;
    length++;
  }
  if (std::cin.fail() && !std::cin.eof())
  {
    std::cout << "Input failed";
    return 1;
  }
  size_t buffer_size = volkovich::getAddMissingSize(input);
  char *missingAsciiString = nullptr;
  try
  {
    missingAsciiString = new char[buffer_size];
  }
  catch (...)
  {
    delete[] missingAsciiString;
    return 1;
  }

  for (size_t i = 0; input[i] != '\0'; i++)
  {
    missingAsciiString[i] = input[i];
  }

  volkovich::addMissing(input, missingAsciiString);
  std::cout << '\n'
            << missingAsciiString << '\n';
  volkovich::deletePindos(input);
  std::cout << input << '\n';

  delete[] missingAsciiString;
  delete[] input;
  return 0;
}
