#include <list>
#include <string>

#include <math.h>

#include "catch2/catch.hpp"

#include "smaep/data_source_samples.hpp"
#include "smaep/parser.hpp"

TEST_CASE("data field", "[data source]") {
  auto expressions = std::list<std::pair<std::string, int64_t>>();
  expressions.push_back({"data[one]", 0});

  auto config = smaep::parser_config<int64_t>::create();
  auto empty_data = smaep::data::empty_source<int64_t>();

  for (const auto &[problem, expected] : expressions) {
    INFO(problem);
    auto ast = smaep::parse<int64_t>(problem, config);
    auto result = ast.value(empty_data);

    CHECK(expected == result);
  }
}

TEST_CASE("with data source", "[data source]") {
  smaep::data::number_names<double> ds;

  auto expressions = std::list<std::tuple<std::string, double>>();
  expressions.push_back({"1 + data[one]", 2});
  expressions.push_back({"data[two] - data[one]", 1});
  expressions.push_back({"abs(data[two])", 2});

  for (const auto &[problem, expected] : expressions) {
    INFO(problem);
    auto ast = smaep::parse<double>(problem);
    auto result = ast.value(ds);

    CHECK(expected == result);
  }
}

TEST_CASE("no data source", "[data source]") {

  auto expressions = std::list<std::tuple<std::string, double>>();
  expressions.push_back({"1 + data[one]", 2});

  for (const auto &[problem, expected] : expressions) {
    INFO(problem);
    auto ast = smaep::parse<double>(problem);
    CHECK_THROWS(ast.value());
  }
}
