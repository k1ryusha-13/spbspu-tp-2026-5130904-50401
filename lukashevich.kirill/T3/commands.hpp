#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <vector>

#include "geom.hpp"

namespace lukashevich {
  void processCommands(std::istream & input, std::ostream & output, std::vector< Polygon > & polygons);
}

#endif
