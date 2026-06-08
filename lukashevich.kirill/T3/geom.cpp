#include "geom.hpp"

#include <cerrno>
#include <cstdlib>
#include <numeric>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <limits>

#include "utils.hpp"

namespace lukashevich {
  namespace detail {
    const std::size_t MIN_POLYGON_VERTEX_COUNT = 3;

    double getDoubleAreaTerm(const Point & lhs, const Point & rhs)
    {
      const double first = static_cast< double >(lhs.x) * rhs.y;
      const double second = static_cast< double >(rhs.x) * lhs.y;
      return first - second;
    }

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

    bool parsePoints(const std::string & line, std::size_t & pos, std::size_t count, std::vector< lukashevich::Point > & points)
    {
      if (count == 0) {
        return true;
      }

      lukashevich::Point point = { 0, 0 };
      if (!parsePoint(line, pos, point)) {
        return false;
      }

      points.push_back(point);
      return parsePoints(line, pos, count - 1, points);
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

double lukashevich::getArea(const Polygon & polygon)
{
  if (polygon.points.size() < detail::MIN_POLYGON_VERTEX_COUNT) {
    return 0.0;
  }

  std::vector< Point > shifted(polygon.points.size());
  std::rotate_copy(polygon.points.begin(),
      std::next(polygon.points.begin()),
      polygon.points.end(),
      shifted.begin());

  std::vector< double > doubleAreaParts(polygon.points.size());
  std::transform(polygon.points.begin(),
      polygon.points.end(),
      shifted.begin(),
      doubleAreaParts.begin(),
      detail::getDoubleAreaTerm);

  const double doubleArea = std::accumulate(doubleAreaParts.begin(),
      doubleAreaParts.end(),
      0.0);

  return std::abs(doubleArea) / 2.0;
}

std::size_t lukashevich::getVertexCount(const Polygon & polygon)
{
  return polygon.points.size();
}

bool lukashevich::hasEvenVertexCount(const Polygon & polygon)
{
  return (getVertexCount(polygon) % 2) == 0;
}

bool lukashevich::hasOddVertexCount(const Polygon & polygon)
{
  return !hasEvenVertexCount(polygon);
}

bool lukashevich::hasVertexCount(const Polygon & polygon, std::size_t count)
{
  return getVertexCount(polygon) == count;
}

bool lukashevich::isVertexCountLess(const Polygon & lhs, const Polygon & rhs)
{
  return getVertexCount(lhs) < getVertexCount(rhs);
}

bool lukashevich::hasAreaLessThan(const Polygon & polygon, double area)
{
  return getArea(polygon) < area;
}

bool lukashevich::isAreaLess(const Polygon & lhs, const Polygon & rhs)
{
  return getArea(lhs) < getArea(rhs);
}

bool lukashevich::parsePolygon(const std::string & line, Polygon & polygon)
{
  std::size_t pos = 0;
  Polygon parsed;
  if (!parsePolygon(line, pos, parsed)) {
    return false;
  }
  if (!hasOnlySpaces(line, pos)) {
    return false;
  }

  polygon = parsed;
  return true;
}

bool lukashevich::areTargetEcho(const Polygon & lhs, const Polygon & rhs, const Polygon & target)
{
  return (lhs == target) && (rhs == target);
}

bool lukashevich::parsePolygon(const std::string & line, std::size_t & pos, Polygon & polygon)
{
  std::size_t count = 0;

  if (!parseSize(line, pos, count)) {
    return false;
  }
  if (count < detail::MIN_POLYGON_VERTEX_COUNT) {
    return false;
  }

  std::vector< Point > points;
  points.reserve(count);

  if (!detail::parsePoints(line, pos, count, points)) {
    return false;
  }

  polygon.points = points;
  return true;
}
