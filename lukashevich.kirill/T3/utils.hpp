#ifndef UTILS_HPP
#define UTILS_HPP

#include <ios>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include "geom.hpp"

namespace lukashevich {
  class IOGuard
  {
  public:
    explicit IOGuard(std::basic_ios< char > & stream);
    ~IOGuard();

  private:
    std::basic_ios< char > & stream_;
    char fill_;
    std::streamsize precision_;
    std::streamsize width_;
    std::ios_base::fmtflags flags_;
  };

  void readPolygons(std::istream & input, std::vector< Polygon > & polygons);
}

#endif
