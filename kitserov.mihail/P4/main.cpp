//14
//LAT_RMV #3
//malloc free
#include <iostream>
#include <iomanip>

namespace kitserov
{
  char * getline(std::istream & in, size_t & s);
  char * miss_letters();
  char * remove_letters();
}

char * kitserov::getline(std::istream & in, size_t & s)
{
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws) {
  	in >> std::noskipws;
  }
  char * data = nullptr;
  data = (char*) malloc(2);
  size_t size = 1;
  char letter;
  size_t i = 0;
  in >> letter;
  while (!(in.fail()) && letter != '\n')
  {
  	if (i == size){
  	  size *= 2;
  	  char * temp = (char*) malloc(size + 1);
  	  for (size_t j = 0; j < size; j++)
  	  {
  	  	temp[j] = data[j];
  	  }
  	  free(data);
  	  data = temp;
  	}
  	data[i] = letter;
  	i++;
  	in >> letter;
  }
  
  s = i;
  data[s] = '\0';
  if (is_skipws) {
  	in >> std::skipws;
  }
  return data;
}

int main()
{
  size_t s;
  char * data = kitserov::getline(std::cin, s);
  if (!data){
  	std::cerr << "Failed read line\n";
  	return 1;
  }

  char * removed_letters = (char*) malloc(s + 1);
  if (!removed_letters){
  	std::cerr << "Failed memory allocation";
  	return 1;
  }
  kitserov::remove_letters(data, removed_letters, s + 1);

  char * missed_letters = (char*) malloc(s + 1);
  if (!missed_letters){
  	std::cerr << "Failed memory allocation";
  	return 1;
  }
  kitserov::miss_letters(data, missed_letters, s + 1);

  std::cout << data << "\n";
  std::cout << s << "\n";
  free(removed_letters);
  free(missed_letters);
  return 0;
}
