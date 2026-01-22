#include <iostream>
#include <istream>
#include <cctype>
#include <new>
namespace loseva
{
  const size_t LATIN_ALPHABET_SIZE = 26;
  char* mergeStrings(const char* first, const char* second, size_t firstLen, size_t secondLen);
  char* readInputLine(std::istream& inputStream, size_t& totalLength);
  void extractUniqueLatins(const char* first, const char* second, char* output, size_t capacity);
  void removeAllLatins(const char* source, char* output, size_t capacity);
  struct ArrayGuard
  {
    char* data = nullptr;
    ArrayGuard(char* p) : data(p) {}
    ~ArrayGuard()
    {
      delete[] data;
    }
  }
}
char* loseva::mergeStrings(const char* first, const char* second, size_t firstLen, size_t secondLen)
{
  char* combined = new char[firstLen + secondLen];
  if (first)
  {
    for (size_t i = 0; i < firstLen; ++i)
    {
      combined[i] = first[i];
    }
  }
  if (second)
  {
    for (size_t i = 0; i < secondLen; ++i)
    {
      combined[firstLen + i] = second[i];
    }
  }
  return combined;
}
char* loseva::readInputLine(std::istream& inputStream, size_t& totalLength)
{
  const size_t chunkCapacity = 10;
  char* accumulatedString = nullptr;
  totalLength = 0;
  char* currentChunk = new char[chunkCapacity];
  ArrayGuard chunkGuard(currentChunk);
  inputStream >> std::noskipws;
  char currentChar = '\0';
  size_t chunkUsage = 0;
  while ((inputStream >> currentChar) && (currentChar != '\n'))
  {
    if (chunkUsage == chunkCapacity)
    {
      char* expanded = mergeStrings(accumulatedString, currentChunk, totalLength, chunkCapacity);
      delete[] accumulatedString;
      accumulatedString = expanded;
      totalLength += chunkCapacity;
      chunkUsage = 0;
    }
    currentChunk[chunkUsage] = currentChar;
    chunkUsage++;
  }
  char* finalString = new char[totalLength + chunkUsage + 1];
  if (accumulatedString)
  {
    for (size_t i = 0; i < totalLength; ++i)
    {
      finalString[i] = accumulatedString[i];
    }
  }
  for (size_t i = 0; i < chunkUsage; ++i)
  {
    finalString[totalLength + i] = currentChunk[i];
  }
  totalLength += chunkUsage;
  finalString[totalLength] = '\0';
  delete[] accumulatedString;
  return finalString;
}
void loseva::extractUniqueLatins(const char* first, const char* second, char* output, size_t capacity)
{
  bool foundLetters[LATIN_ALPHABET_SIZE] = {false};
  size_t writePos = 0;
  auto markLatins = [&](const char* str)
  {
    if (!str)
    {
      return;
    }
    for (size_t i = 0; str[i] != '\0'; ++i)
    {
      if (std::isalpha(static_cast<unsigned char>(str[i])))
      {
        foundLetters[std::tolower(static_cast<unsigned char>(str[i])) - 'a'] = true;
      }
    }
  }
  markLatins(first);
  markLatins(second);
  for (size_t i = 0; i < LATIN_ALPHABET_SIZE; ++i)
  {
    if (foundLetters[i])
    {
      if (writePos + 1 >= capacity)
      {
        throw std::bad_alloc();
      }
      output[writePos] = static_cast<char>('a' + i);
      writePos++;
    }
  }
  output[writePos] = '\0';
}
void loseva::removeAllLatins(const char* source, char* output, size_t capacity)
{
  if (!source)
  {
    if (capacity > 0)
    {
      output[0] = '\0';
    }
    return;
  }
  size_t writePos = 0;
  for (size_t i = 0; source[i] != '\0'; ++i)
  {
    if (!std::isalpha(static_cast<unsigned char>(source[i])))
    {
      if (writePos + 1 >= capacity)
      {
        throw std::bad_alloc();
      }
      output[writePos] = source[i];
      writePos++;
    }
  }
  output[writePos] = '\0';
}
int main()
{
  char* inputData = nullptr;
  char* filteredData = nullptr;
  try
  {
    size_t length = 0;
    inputData = loseva::readInputLine(std::cin, length);
    char uniqueChars[loseva::LATIN_ALPHABET_SIZE + 1] = {'\0'};
    filteredData = new char[length + 1];
    loseva::extractUniqueLatins(inputData, "defghk", uniqueChars, sizeof(uniqueChars));
    std::cout << uniqueChars << "\n";
    loseva::removeAllLatins(inputData, filteredData, length + 1);
    std::cout << filteredData << "\n";
    delete[] inputData;
    delete[] filteredData;
  }
  catch (const std::bad_alloc&)
  {
    delete[] inputData;
    delete[] filteredData;
    std::cerr << "Memory allocation error\n";
    return 1;
  }
  return 0;
}
