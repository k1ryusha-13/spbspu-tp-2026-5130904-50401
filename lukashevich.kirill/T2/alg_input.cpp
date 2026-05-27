#include "alg_input.hpp"

#include <algorithm>
#include <cerrno>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <string>

namespace lukashevich {
  namespace detail {
    bool isDigit(char symbol);
    bool parseSciDouble(const std::string &text, double &value);
    bool parseBinaryUll(const std::string &text, unsigned long long &value);
    bool readValueToken(std::istream &in, std::string &token);
  }
}

lukashevich::IOGuard::IOGuard(std::basic_ios< char > &stream):
  stream_(stream),
  precision_(stream.precision()),
  width_(stream.width()),
  flags_(stream.flags()),
  fill_(stream.fill())
{}

lukashevich::IOGuard::~IOGuard()
{
  stream_.precision(precision_);
  stream_.width(width_);
  stream_.flags(flags_);
  stream_.fill(fill_);
}

bool lukashevich::detail::isDigit(char symbol)
{
  return ('0' <= symbol) && (symbol <= '9');
}

bool lukashevich::detail::parseSciDouble(const std::string &text, double &value)
{
  if (text.empty()) {
    return false;
  }

  std::size_t pos = 0;
  if (text[pos] == '-') {
    ++pos;
  }
  if ((pos == text.size()) || !isDigit(text[pos])) {
    return false;
  }

  while ((pos < text.size()) && isDigit(text[pos])) {
    ++pos;
  }

  if ((pos == text.size()) || (text[pos] != '.')) {
    return false;
  }
  ++pos;

  if ((pos == text.size()) || !isDigit(text[pos])) {
    return false;
  }

  while ((pos < text.size()) && isDigit(text[pos])) {
    ++pos;
  }

  if ((pos == text.size()) || (text[pos] != 'e')) {
    return false;
  }
  ++pos;

  if ((pos == text.size()) || ((text[pos] != '+') && (text[pos] != '-'))) {
    return false;
  }
  ++pos;

  if ((pos == text.size()) || !isDigit(text[pos])) {
    return false;
  }

  while ((pos < text.size()) && isDigit(text[pos])) {
    ++pos;
  }

  if (pos != text.size()) {
    return false;
  }

  errno = 0;
  char *end = nullptr;
  const double parsedValue = std::strtod(text.c_str(), &end);

  if ((end != (text.c_str() + text.size())) || (errno == ERANGE)) {
    return false;
  }
  if (!std::isfinite(parsedValue)) {
    return false;
  }

  value = parsedValue;
  return true;
}

bool lukashevich::detail::parseBinaryUll(const std::string &text,
    unsigned long long &value)
{
  const std::size_t minSize = 3;
  if (text.size() < minSize) {
    return false;
  }

  if ((text[0] != '0') || ((text[1] != 'b') && (text[1] != 'B'))) {
    return false;
  }

  const std::size_t firstDigit = 2;
  if ((text.size() > (firstDigit + 1)) && (text[firstDigit] == '0')) {
    return false;
  }

  unsigned long long result = 0;
  const unsigned long long maxValue = std::numeric_limits< unsigned long long >::max();

  for (std::size_t i = firstDigit; i < text.size(); ++i) {
    if ((text[i] != '0') && (text[i] != '1')) {
      return false;
    }

    const unsigned long long bit = static_cast< unsigned long long >(text[i] - '0');
    if (result > ((maxValue - bit) / 2)) {
      return false;
    }

    result = (result * 2) + bit;
  }

  value = result;
  return true;
}

bool lukashevich::detail::readValueToken(std::istream &in, std::string &token)
{
  token.clear();

  while (std::isspace(in.peek())) {
    in.get();
  }

  char symbol = 0;
  while (in.get(symbol) && (symbol != ':')) {
    token += symbol;
  }

  if (!in) {
    return false;
  }

  in.unget();

  while (!token.empty() && std::isspace(token.back())) {
    token.erase(token.size() - 1);
  }

  return !token.empty();
}

std::istream &lukashevich::operator>>(std::istream &in, DblSci &data)
{
  std::string token;
  if (!detail::readValueToken(in, token)) {
    in.setstate(std::ios_base::failbit);
    return in;
  }

  double value = 0.0;
  if (!detail::parseSciDouble(token, value)) {
    in.setstate(std::ios_base::failbit);
    return in;
  }

  data.value = value;
  return in;
}

std::ostream &lukashevich::operator<<(std::ostream &out, const DblSci &data)
{
  IOGuard guard(out);

  double value = data.value;
  if (std::fabs(value) < 1e-12) {
    out << "0.0e+0";
    return out;
  }

  if (value < 0.0) {
    out << '-';
    value = -value;
  }

  int exponent = 0;

  while (value >= 10.0) {
    value /= 10.0;
    ++exponent;
  }

  while ((value < 1.0) && (value > 0.0)) {
    value *= 10.0;
    --exponent;
  }

  double mantissa = std::floor(value * 10.0 + 0.5) / 10.0;

  if (std::fabs(mantissa - 10.0) < 1e-12) {
    mantissa = 1.0;
    ++exponent;
  }

  out << std::fixed << std::setprecision(1) << mantissa << 'e';

  if (exponent >= 0) {
    out << '+';
  }

  out << exponent;
  return out;
}

bool lukashevich::operator<(const DblSci &left, const DblSci &right)
{
  return left.value < right.value;
}

bool lukashevich::operator==(const DblSci &left, const DblSci &right)
{
  return std::fabs(left.value - right.value) < 1e-12;
}

std::istream &lukashevich::operator>>(std::istream &in, UllBin &data)
{
  std::string token;
  if (!detail::readValueToken(in, token)) {
    in.setstate(std::ios_base::failbit);
    return in;
  }

  unsigned long long value = 0;
  if (!detail::parseBinaryUll(token, value)) {
    in.setstate(std::ios_base::failbit);
    return in;
  }

  data.value = value;
  return in;
}

std::ostream &lukashevich::operator<<(std::ostream &out, const UllBin &data)
{
  if (data.value == 0) {
    out << "0b0";
    return out;
  }

  unsigned long long value = data.value;
  std::string result;

  while (value != 0) {
    const char bit = static_cast< char >('0' + (value % 2));
    result += bit;
    value /= 2;
  }

  std::reverse(result.begin(), result.end());

  out << "0b" << result;
  return out;
}

bool lukashevich::operator<(const UllBin &left, const UllBin &right)
{
  return left.value < right.value;
}

bool lukashevich::operator==(const UllBin &left, const UllBin &right)
{
  return left.value == right.value;
}
