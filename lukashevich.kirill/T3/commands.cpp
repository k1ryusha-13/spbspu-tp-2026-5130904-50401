#include "commands.hpp"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include "utils.hpp"

namespace lukashevich {
  namespace detail {
    const char * const INVALID_COMMAND = "<INVALID COMMAND>";
    const int AREA_PRECISION = 1;
    const std::size_t MIN_VERTEX_COUNT = 3;

    template< class Predicate >
    double getAreaSum(const std::vector< Polygon > & polygons, Predicate predicate)
    {
      std::vector< Polygon > selected;
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(selected), predicate);

      std::vector< double > areas(selected.size());
      std::transform(selected.begin(), selected.end(), areas.begin(), getArea);
      return std::accumulate(areas.begin(), areas.end(), 0.0);
    }

    double getAreaSum(const std::vector< Polygon > & polygons)
    {
      std::vector< double > areas(polygons.size());
      std::transform(polygons.begin(), polygons.end(), areas.begin(), getArea);
      return std::accumulate(areas.begin(), areas.end(), 0.0);
    }

    bool isCorrectVertexCount(std::size_t count)
    {
      return count >= MIN_VERTEX_COUNT;
    }

    void printInvalidCommand(std::ostream & output)
    {
      output << INVALID_COMMAND << '\n';
    }

    void printArea(std::ostream & output, double area)
    {
      IOGuard guard(output);
      output << std::fixed << std::setprecision(AREA_PRECISION) << area << '\n';
    }

    bool readSingleParameter(const std::string & line, std::size_t & pos, std::string & parameter)
    {
      parameter = readToken(line, pos);
      return !parameter.empty() && hasOnlySpaces(line, pos);
    }

    void doArea(const std::string & line, std::size_t pos, std::ostream & output, const std::vector< Polygon > & polygons)
    {
      std::string parameter;
      if (!readSingleParameter(line, pos, parameter)) {
        printInvalidCommand(output);
        return;
      }

      if (parameter == "EVEN") {
        printArea(output, getAreaSum(polygons, hasEvenVertexCount));
      } else if (parameter == "ODD") {
        printArea(output, getAreaSum(polygons, hasOddVertexCount));
      } else if (parameter == "MEAN") {
        if (polygons.empty()) {
          printInvalidCommand(output);
          return;
        }
        printArea(output, getAreaSum(polygons) / polygons.size());
      } else {
        std::size_t vertexCount = 0;
        if (!parseSizeToken(parameter, vertexCount)) {
          printInvalidCommand(output);
          return;
        }
        if (!isCorrectVertexCount(vertexCount)) {
          printInvalidCommand(output);
          return;
        }
        const double area = getAreaSum(polygons, std::bind(hasVertexCount, std::placeholders::_1, vertexCount));
        printArea(output, area);
      }
    }

    void doMax(const std::string & line, std::size_t pos, std::ostream & output, const std::vector< Polygon > & polygons)
    {
      std::string parameter;
      if (!readSingleParameter(line, pos, parameter)) {
        printInvalidCommand(output);
        return;
      }
      if (polygons.empty()) {
        printInvalidCommand(output);
        return;
      }

      if (parameter == "AREA") {
        const std::vector< Polygon >::const_iterator result = std::max_element(polygons.begin(), polygons.end(), isAreaLess);
        printArea(output, getArea(*result));
      } else if (parameter == "VERTEXES") {
        const std::vector< Polygon >::const_iterator result = std::max_element(polygons.begin(), polygons.end(), isVertexCountLess);
        output << getVertexCount(*result) << '\n';
      } else {
        printInvalidCommand(output);
      }
    }

    void doMin(const std::string & line, std::size_t pos, std::ostream & output, const std::vector< Polygon > & polygons)
    {
      std::string parameter;
      if (!readSingleParameter(line, pos, parameter)) {
        printInvalidCommand(output);
        return;
      }
      if (polygons.empty()) {
        printInvalidCommand(output);
        return;
      }

      if (parameter == "AREA") {
        const std::vector< Polygon >::const_iterator result = std::min_element(polygons.begin(), polygons.end(), isAreaLess);
        printArea(output, getArea(*result));
      } else if (parameter == "VERTEXES") {
        const std::vector< Polygon >::const_iterator result = std::min_element(polygons.begin(), polygons.end(), isVertexCountLess);
        output << getVertexCount(*result) << '\n';
      } else {
        printInvalidCommand(output);
      }
    }

    void doCount(const std::string & line, std::size_t pos, std::ostream & output, const std::vector< Polygon > & polygons)
    {
      std::string parameter;
      if (!readSingleParameter(line, pos, parameter)) {
        printInvalidCommand(output);
        return;
      }

      if (parameter == "EVEN") {
        output << std::count_if(polygons.begin(), polygons.end(), hasEvenVertexCount) << '\n';
      } else if (parameter == "ODD") {
        output << std::count_if(polygons.begin(), polygons.end(), hasOddVertexCount) << '\n';
      } else {
        std::size_t vertexCount = 0;
        if (!parseSizeToken(parameter, vertexCount)) {
          printInvalidCommand(output);
          return;
        }
        if (!isCorrectVertexCount(vertexCount)) {
          printInvalidCommand(output);
          return;
        }
        output << std::count_if(polygons.begin(),
            polygons.end(),
            std::bind(hasVertexCount, std::placeholders::_1, vertexCount)) << '\n';
      }
    }

    void doLessArea(const std::string & line, std::size_t pos, std::ostream & output, const std::vector< Polygon > & polygons)
    {
      Polygon polygon;
      if (!parsePolygon(line, pos, polygon)) {
        printInvalidCommand(output);
        return;
      }
      if (!hasOnlySpaces(line, pos)) {
        printInvalidCommand(output);
        return;
      }

      const double area = getArea(polygon);
      output << std::count_if(polygons.begin(),
          polygons.end(),
          std::bind(hasAreaLessThan, std::placeholders::_1, area)) << '\n';
    }

    void doRmEcho(const std::string & line, std::size_t pos, std::ostream & output, std::vector< Polygon > & polygons)
    {
      Polygon target;
      if (!parsePolygon(line, pos, target)) {
        printInvalidCommand(output);
        return;
      }
      if (!hasOnlySpaces(line, pos)) {
        printInvalidCommand(output);
        return;
      }

      const std::vector< Polygon >::iterator newEnd = std::unique(polygons.begin(),
        polygons.end(),
        std::bind(areTargetEcho, std::placeholders::_1,
        std::placeholders::_2,
        std::cref(target)));
      const std::size_t removedCount = static_cast< std::size_t >(
          std::distance(newEnd, polygons.end()));
      polygons.erase(newEnd, polygons.end());
      output << removedCount << '\n';
    }

    void executeCommand(const std::string & line, std::ostream & output, std::vector< Polygon > & polygons)
    {
      std::size_t pos = 0;
      const std::string command = readToken(line, pos);

      if (command.empty()) {
        return;
      }

      if (command == "AREA") {
        doArea(line, pos, output, polygons);
      } else if (command == "MAX") {
        doMax(line, pos, output, polygons);
      } else if (command == "MIN") {
        doMin(line, pos, output, polygons);
      } else if (command == "COUNT") {
        doCount(line, pos, output, polygons);
      } else if (command == "LESSAREA") {
        doLessArea(line, pos, output, polygons);
      } else if (command == "RMECHO") {
        doRmEcho(line, pos, output, polygons);
      } else {
        printInvalidCommand(output);
      }

      printInvalidCommand(output);
    }
  }
}

void lukashevich::processCommands(std::istream & input, std::ostream & output, std::vector< Polygon > & polygons)
{
  std::string line;

  if (!std::getline(input, line)) {
    return;
  }

  detail::executeCommand(line, output, polygons);
  processCommands(input, output, polygons);
}
