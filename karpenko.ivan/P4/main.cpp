#include "array_utils.hpp"
#include "string_functions.hpp"
#include <iostream>
#include <cstring>
#include <cstddef>

int main()
{
  size_t wordCount = 0;
  char** words = nullptr;
  
  try
  {
    words = karpenko::readWords(std::cin, wordCount);
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Error: cannot allocate memory for words\n";
    return 1;
  }
  catch (const std::runtime_error& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  catch (...)
  {
    std::cerr << "Error: unexpected error while reading words\n";
    return 1;
  }
  
  if (words == nullptr || wordCount == 0)
  {
    std::cerr << "Error: no words found\n";
    return 1;
  }
  
  const char line2[] = "def_";
  const size_t line2Length = sizeof(line2) - 1;
  
  for (size_t i = 0; i < wordCount; ++i)
  {
    const size_t wordLen = std::strlen(words[i]);
    const size_t resultSize = wordLen + line2Length + 1;
    
    char* resultUni = nullptr;
    char* resultShr = nullptr;
    
    try
    {
      resultUni = new char[resultSize];
      resultShr = new char[karpenko::ALPHABET_RESULT_SIZE];
      
      karpenko::uniTwo(words[i], wordLen, line2,
        line2Length, resultUni, resultSize);
      std::cout << resultUni << '\n';
      
      karpenko::shrSym(words[i], resultShr);
      std::cout << resultShr << '\n';
    }
    catch (const std::bad_alloc&)
    {
      std::cerr << "Error: cannot allocate memory for results\n";
      delete[] resultUni;
      delete[] resultShr;
      
      for (size_t j = 0; j < wordCount; ++j)
      {
        delete[] words[j];
      }
      delete[] words;
      return 1;
    }
    catch (...)
    {
      std::cerr << "Error: unexpected error while processing words\n";
      delete[] resultUni;
      delete[] resultShr;
      
      for (size_t j = 0; j < wordCount; ++j)
      {
        delete[] words[j];
      }
      delete[] words;
      return 1;
    }
    
    delete[] resultUni;
    delete[] resultShr;
  }
  
  try
  {
    for (size_t i = 0; i < wordCount; ++i)
    {
      delete[] words[i];
    }
    delete[] words;
  }
  catch (...)
  {
    std::cerr << "Warning: error while freeing memory\n";
    return 1;
  }
  
  return 0;
}
