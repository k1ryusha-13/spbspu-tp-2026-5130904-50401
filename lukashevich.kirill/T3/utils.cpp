#include "utils.hpp"

lukashevich::IOGuard::IOGuard(std::basic_ios< char > & stream):
  stream_(stream),
  fill_(stream.fill()),
  precision_(stream.precision()),
  width_(stream.width()),
  flags_(stream.flags())
{}

lukashevich::IOGuard::~IOGuard()
{
  stream_.fill(fill_);
  stream_.precision(precision_);
  stream_.width(width_);
  stream_.flags(flags_);
}

bool lukashevich::isSpace(char symbol)
{
  return (symbol == ' ') || (symbol == '\t') || (symbol == '\r');
}

std::size_t lukashevich::skipSpaces(const std::string & line, std::size_t pos)
{
  if ((pos < line.size()) && isSpace(line[pos])) {
    return skipSpaces(line, pos + 1);
  }
  return pos;
}

bool lukashevich::hasOnlySpaces(const std::string & line, std::size_t pos)
{
  return skipSpaces(line, pos) == line.size();
}

std::string lukashevich::readToken(const std::string & line, std::size_t & pos)
{
  const std::size_t begin = skipSpaces(line, pos);
  const std::size_t end = line.find_first_of(" \t\r", begin);

  if (end == std::string::npos) {
    pos = line.size();
    return line.substr(begin);
  }

  pos = end;
  return line.substr(begin, end - begin);
}

void lukashevich::readPolygons(std::istream &, std::vector< Polygon > &)
{}
