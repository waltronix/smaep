#pragma once

#include <string>

namespace smaep::data {
template <typename TValue> class data_source_interface {
public:
  virtual TValue get_value_for(const std::string &key) const = 0;
};
} // namespace smaep::data
