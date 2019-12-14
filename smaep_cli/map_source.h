#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "i_data_source.h"

namespace smaep::data {
template <typename TValue>
class map_source : public i_data_source<TValue> {
  std::map<std::string, TValue> m_data;

 public:
  void add(std::string key, TValue value) { m_data[key] = value; }

  TValue get_value_for(const std::string& key) const override {
    return m_data.at(key);
  }

  static map_source create_top_ten() {
    map_source result;
    result.add("zero", 0);
    result.add("one", 1);
    result.add("two", 2);
    result.add("three", 3);
    result.add("four", 4);
    result.add("five", 5);
    result.add("six", 6);
    result.add("seven", 7);
    result.add("eight", 8);
    result.add("nine", 9);
    result.add("ten", 10);

    return result;
  }
};
}  // namespace smaep::data
