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

    void printInvalidCommand(std::ostream & output)
    {
      output << INVALID_COMMAND << '\n';
    }

    void executeCommand(const std::string & line,
        std::ostream & output,
        std::vector< Polygon > & polygons)
    {
      std::size_t pos = 0;
      const std::string command = readToken(line, pos);

      if (command.empty()) {
        return;
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
