#pragma once

#include <map>

#include "data_source_interface.h"

namespace smaep::data {
template <typename TValue>
class empty_source : public data_source_interface<TValue> {
public:
  TValue get_value_for(const std::string &) const final { return 0; }
};

template <typename TValue>
class number_names : public smaep::data::data_source_interface<TValue> {
  std::map<std::string, TValue> data{
      {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};

public:
  TValue get_value_for(const std::string &key) const final {
    return data.at(key);
  }
};
} // namespace smaep::data
