#include "geom.hpp"

#include <cerrno>
#include <cstdlib>

#include "utils.hpp"

namespace lukashevich {
  namespace detail {
    bool parseChar(const std::string & line, std::size_t & pos, char expected)
    {
      pos = skipSpaces(line, pos);
      if ((pos < line.size()) && (line[pos] == expected)) {
        ++pos;
        return true;
      }
      return false;
    }

    bool parseInt(const std::string & line, std::size_t & pos, int & value)
    {
      pos = skipSpaces(line, pos);
      errno = 0;
      const char * begin = line.c_str() + pos;
      char * end = nullptr;
      const long result = std::strtol(begin, &end, 10);

      if ((begin == end) || (errno == ERANGE)) {
        return false;
      }
      if (result < std::numeric_limits< int >::min()) {
        return false;
      }
      if (result > std::numeric_limits< int >::max()) {
        return false;
      }

      pos = static_cast< std::size_t >(end - line.c_str());
      value = static_cast< int >(result);
      return true;
    }

    bool parsePoint(const std::string & line, std::size_t & pos, Point & point)
    {
      int x = 0;
      int y = 0;
      if (!parseChar(line, pos, '(')) {
        return false;
      }
      if (!parseInt(line, pos, x)) {
        return false;
      }
      if (!parseChar(line, pos, ';')) {
        return false;
      }
      if (!parseInt(line, pos, y)) {
        return false;
      }
      if (!parseChar(line, pos, ')')) {
        return false;
      }

      point = Point{ x, y };
      return true;
    }
  }
}

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
