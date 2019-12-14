#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdefaulted-function-deleted"
#endif
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonpath/json_query.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include "exceptions.hpp"
#include "json_source.h"

jsoncons::json smaep::data::json_source_base::query(
    const std::string& json_path) const {
  return jsoncons::jsonpath::json_query(m_json, json_path);
}

jsoncons::json smaep::data::json_source_base::get_only_one(
    const std::string& json_path) const {
  auto values = query(json_path);
  if (0 == values.size()) {
    throw data_exception("not enough values");
  } else if (values.size() > 1) {
    throw data_exception("too much values");
  }
  return values[0];
}