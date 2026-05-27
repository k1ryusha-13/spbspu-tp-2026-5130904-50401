#include <iostream>
#include <algorithm>
#include <iterator>
#include <limits>
#include <vector>

#include "alg_input.hpp"

int main()
{
  using Data = lukashevich::DataStruct;

  std::vector< Data > data;
  using InputIterator = std::istream_iterator< Data >;

  while (!std::cin.eof()) {
    std::copy(InputIterator(std::cin), InputIterator(), std::back_inserter(data));

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());

  using OutputIterator = std::ostream_iterator< Data >;
  std::copy(data.begin(), data.end(), OutputIterator(std::cout, "\n"));

  return 0;
}
