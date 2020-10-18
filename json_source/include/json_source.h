#pragma once

#include <cstdint>
#include <string>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdefaulted-function-deleted"
#endif
#include <jsoncons/json.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include "smaep/exceptions.hpp"
#include "smaep/i_data_source.h"

namespace smaep::data {
class json_source_base {
  jsoncons::json m_json;

 public:
  json_source_base(const std::string& json)
      : m_json(jsoncons::json::parse(json)) {}

  jsoncons::json query(const std::string& json_path) const;

  jsoncons::json get_only_one(const std::string& json_path) const;
};

template <typename TValue>
class json_source : public i_data_source<TValue>, private json_source_base {
 public:
  using json_source_base::json_source_base;

  TValue get_value_for(const std::string& key) const override {
    return get_only_one(key).template as<TValue>();
  }
};
}  // namespace smaep::data
