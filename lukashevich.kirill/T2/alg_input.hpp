#ifndef ALG_INPUT_HPP
#define ALG_INPUT_HPP

#include <iostream>
#include <string>

namespace lukashevich {
  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

  bool compareData(const DataStruct &left, const DataStruct &right);
  std::istream &operator>>(std::istream &in, DataStruct &data);
  std::ostream &operator<<(std::ostream &out, const DataStruct &data);
}

#endif
