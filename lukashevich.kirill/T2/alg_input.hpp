#ifndef ALG_INPUT_HPP
#define ALG_INPUT_HPP

#include <iostream>
#include <string>
#include <vector>

namespace lukashevich {
  struct DblSci
  {
    double value;
  };

  struct UllBin
  {
    unsigned long long value;
  };

  struct DataStruct
  {
    DblSci key1;
    UllBin key2;
    std::string key3;
  };

  struct Delimiter
  {
    char expected;
  };

  struct KeyValueInput
  {
    std::string key;
    std::vector< bool > &usedKeys;
    DataStruct &data;
  };

  class IOGuard
  {
  public:
    explicit IOGuard(std::basic_ios< char > &stream);
    ~IOGuard();

  private:
    std::basic_ios< char > &stream_;
    std::streamsize precision_;
    std::streamsize width_;
    std::basic_ios< char >::fmtflags flags_;
    char fill_;
  };

  std::istream &operator>>(std::istream &in, DblSci &data);
  std::ostream &operator<<(std::ostream &out, const DblSci &data);
  bool operator<(const DblSci &left, const DblSci &right);
  bool operator==(const DblSci &left, const DblSci &right);

  std::istream &operator>>(std::istream &in, UllBin &data);
  std::ostream &operator<<(std::ostream &out, const UllBin &data);
  bool operator<(const UllBin &left, const UllBin &right);
  bool operator==(const UllBin &left, const UllBin &right);

  std::istream &operator>>(std::istream &in, Delimiter &&data);
  std::istream &operator>>(std::istream &in, KeyValueInput &&data);

  std::istream &operator>>(std::istream &in, DataStruct &data);
  std::ostream &operator<<(std::ostream &out, const DataStruct &data);
  bool operator<(const DataStruct &left, const DataStruct &right);
}

#endif
