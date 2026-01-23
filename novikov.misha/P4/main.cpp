#include <iostream>

using namespace std;

bool hasChar(const char* str, char ch)
{
  for (int i = 0; str[i] != '\0'; ++i)
  {
    if (str[i] == ch)
    {
      return true;
    }
  }
  return false;
}

void getUniqueSymbols(const char* str1, const char* str2, char* buffer, size_t bufSize)
{
  int idx = 0;
  
  for (int i = 0; str1[i] != '\0' && idx < bufSize - 1; ++i)
  {
    char c = str1[i];
    if (!hasChar(str2, c) && !hasChar(buffer, c))
    {
      buffer[idx] = c;
      ++idx;
    }
  }
  
  for (int i = 0; str2[i] != '\0' && idx < bufSize - 1; ++i)
  {
    char c = str2[i];
    if (!hasChar(str1, c) && !hasChar(buffer, c))
    {
      buffer[idx] = c;
      ++idx;
    }
  }
  
  buffer[idx] = '\0';
}

int main()
{
  const size_t BUFFER_SIZE = 256;
  char buffer[BUFFER_SIZE];
  char input[BUFFER_SIZE];
  
  const char* secondString = "abc_ef";
  
  if (!cin.getline(input, BUFFER_SIZE))
  {
    return 1;
  }
  
  getUniqueSymbols(input, secondString, buffer, BUFFER_SIZE);
  
  cout << buffer << endl;
  
  return 0;
}
