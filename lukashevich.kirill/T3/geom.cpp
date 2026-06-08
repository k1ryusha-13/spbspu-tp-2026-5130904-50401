#include "geom.hpp"

bool lukashevich::operator==(const Point & lhs, const Point & rhs)
{
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool lukashevich::operator!=(const Point & lhs, const Point & rhs)
{
  return !(lhs == rhs);
}

bool lukashevich::operator==(const Polygon & lhs, const Polygon & rhs)
{
  return lhs.points == rhs.points;
}

bool lukashevich::operator!=(const Polygon & lhs, const Polygon & rhs)
{
  return !(lhs == rhs);
}

double lukashevich::getArea(const Polygon &)
{
  return 0.0;
}

std::size_t lukashevich::getVertexCount(const Polygon & polygon)
{
  return polygon.points.size();
}

bool lukashevich::parsePolygon(const std::string &, Polygon &)
{
  return false;
}

bool lukashevich::parsePolygon(const std::string &, std::size_t &, Polygon &)
{
  return false;
}
