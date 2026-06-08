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

void lukashevich::readPolygons(std::istream &, std::vector< Polygon > &)
{}
