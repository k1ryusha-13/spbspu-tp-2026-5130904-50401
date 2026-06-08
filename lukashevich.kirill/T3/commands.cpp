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
      std::copy_if(polygons.begin(),
          polygons.end(),
          std::back_inserter(selected),
          predicate);

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

    void printInvalidCommand(std::ostream & output)
    {
      output << INVALID_COMMAND << '\n';
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
      } else {
        printInvalidCommand(output);
      }

      printInvalidCommand(output);
    }

     void printArea(std::ostream & output, double area)
    {
      IOGuard guard(output);
      output << std::fixed << std::setprecision(AREA_PRECISION) << area << '\n';
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
