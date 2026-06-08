#include <iostream>
#include <fstream>
#include <vector>

#include "commands.hpp"
#include "geom.hpp"
#include "utils.hpp"

int main(int argc, char * argv[])
{
  const int expectedArgc = 2;
  if (argc != expectedArgc) {
    std::cerr << "wrong filename\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input) {
    std::cerr << "can't open\n";
    return 1;
  }

  std::vector< lukashevich::Polygon > polygons;
  lukashevich::readPolygons(input, polygons);
  lukashevich::processCommands(std::cin, std::cout, polygons);
  return 0;
}
