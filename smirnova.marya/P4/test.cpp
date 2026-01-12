#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstring>

namespace smirnova
{
  char** getWords(std::istream &in, size_t& size, bool(*isDelimiter)(char));
}

// Функция-разделитель: проверяет, является ли символ пробельным
bool isSpaceDelimiter(char ch)
{
  return std::isspace(static_cast<unsigned char>(ch));
}

// Функция для подсчёта символов в слове
size_t countWordLength(const char* word)
{
  size_t length = 0;
  while (word[length] != '\0') {
    length++;
  }
  return length;
}

// Функция для копирования слова
char* copyWord(const char* source)
{
  size_t length = countWordLength(source);
  char* dest = reinterpret_cast<char*>(malloc(length + 1));
  if (dest) {
    for (size_t i = 0; i <= length; i++) {
      dest[i] = source[i];
    }
  }
  return dest;
}

// Функция для считывания слов по разделителю
char** smirnova::getWords(std::istream &in, size_t& size, bool(*isDelimiter)(char))
{
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws) {
    in >> std::noskipws;
  }

  size_t arrayCapacity = 16;
  size_t wordCount = 0;

  char** words = reinterpret_cast<char**>(malloc(arrayCapacity * sizeof(char*)));
  if (!words) {
    return 0;
  }

  size_t bufferCapacity = 32;
  size_t bufferLength = 0;

  char* wordBuffer = reinterpret_cast<char*>(malloc(bufferCapacity));
  if (!wordBuffer) {
    free(words);
    return 0;
  }

  char ch = '\0';
  bool inWord = false;

  while (in.get(ch))
  {
    if (isDelimiter(ch))
    {
      if (inWord && bufferLength > 0)
      {
        wordBuffer[bufferLength] = '\0';
        char* word = copyWord(wordBuffer);
        
        if (wordCount >= arrayCapacity)
        {
          size_t newArrayCapacity = arrayCapacity * 2;
          char** newWordsArray = reinterpret_cast<char**>(realloc(words, newArrayCapacity * sizeof(char*)));
          if (newWordsArray) {
            words = newWordsArray;
            arrayCapacity = newArrayCapacity;
          }
        }
        
        if (word) {
          words[wordCount++] = word;
        }
        bufferLength = 0;
        inWord = false;
      }
    }
    else
    {
      if (bufferLength + 1 >= bufferCapacity)
      {
        size_t newBufferCapacity = bufferCapacity * 2;
        char* newBuffer = reinterpret_cast<char*>(realloc(wordBuffer, newBufferCapacity));
        if (newBuffer) {
          wordBuffer = newBuffer;
          bufferCapacity = newBufferCapacity;
        }
      }
      wordBuffer[bufferLength++] = ch;
      inWord = true;
    }
    
    if (ch == '\n') break;
  }

  if (inWord && bufferLength > 0)
  {
    wordBuffer[bufferLength] = '\0';
    char* word = copyWord(wordBuffer);
    if (word) {
      words[wordCount++] = word;
    }
  }

  free(wordBuffer);

  if (is_skipws) {
    in >> std::skipws;
  }

  char** result = reinterpret_cast<char**>(malloc(wordCount * sizeof(char*)));
  for (size_t i = 0; i < wordCount; i++) {
    result[i] = words[i];
  }
  free(words);
  size = wordCount;
  return result;
}

int main()
{
  std::cout << "Тестирование getWords с разными разделителями:" << std::endl;
  std::cout << "Введите текст с пробелами, табуляцией и переводами строк:" << std::endl;
  
  size_t wordCount = 0;
  char** words = smirnova::getWords(std::cin, wordCount, isSpaceDelimiter);

  if (!words) {
    std::cerr << "Ошибка при чтении слов\n";
    return 1;
  }

  std::cout << "\nНайдено слов: " << wordCount << std::endl;

  for (size_t i = 0; i < wordCount; i++) {
    std::cout << "Слово " << (i + 1) << ": \"" << words[i] << "\"" << std::endl;
  }

  for (size_t i = 0; i < wordCount; i++) {
    free(words[i]);
  }
  free(words);

  return 0;
}
