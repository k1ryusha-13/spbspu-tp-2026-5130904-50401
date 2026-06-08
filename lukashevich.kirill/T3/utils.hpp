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

  bool isSpace(char symbol);
  std::size_t skipSpaces(const std::string & line, std::size_t pos);
  bool hasOnlySpaces(const std::string & line, std::size_t pos);
  std::string readToken(const std::string & line, std::size_t & pos);

  void readPolygons(std::istream & input, std::vector< Polygon > & polygons);
}

#endif
