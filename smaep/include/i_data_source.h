#pragma once

#include <string>

namespace smaep::data {
template <typename TValue>
class i_data_source {
 public:
  virtual TValue get_value_for(const std::string& key) const = 0;
};

template <typename TValue>
class empty_source : public i_data_source<TValue> {
 public:
  TValue get_value_for(const std::string&) const final { return 0; }
};
}  // namespace smaep::data
