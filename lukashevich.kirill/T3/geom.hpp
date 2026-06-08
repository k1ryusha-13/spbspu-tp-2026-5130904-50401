#ifndef GEOM_HPP
#define GEOM_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace lukashevich {
  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  bool operator==(const Point & lhs, const Point & rhs);
  bool operator!=(const Point & lhs, const Point & rhs);
  bool operator==(const Polygon & lhs, const Polygon & rhs);
  bool operator!=(const Polygon & lhs, const Polygon & rhs);

  double getArea(const Polygon & polygon);
  std::size_t getVertexCount(const Polygon & polygon);

  bool parsePolygon(const std::string & line, Polygon & polygon);
  bool parsePolygon(const std::string & line, std::size_t & pos, Polygon & polygon);
}

#endif
