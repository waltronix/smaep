#include <list>
#include <string>

#include "catch2/catch.hpp"

#include "parser.hpp"

// #include "data/map_source.h"
// #include "sample_expressions.h"

TEST_CASE("parse_to_double_ast", "parser") {

  auto expressions = std::list<std::tuple<std::string, double>>();
  // expressions.push_back({"1", 1 });
  expressions.push_back({"1 + 1", 2});
  expressions.push_back({"1 * 1", 1});
  expressions.push_back({"(1 * 1)", 1});
  expressions.push_back({"2 * (3 + 1)", 8});
  expressions.push_back({"2 * 3 + 1", 7});
  expressions.push_back({"1 * 2 + 3", 5});
  expressions.push_back({"1 + 2 * 3", 7});
  expressions.push_back({"1 / 2", 0.5});
  expressions.push_back({"1.5 / 2", 0.75});
  expressions.push_back({"abs(-2)", 2});
  expressions.push_back({"sin(1/3)^2 + cos(1/3)^2", 1});

  // auto expressions = samples::get_double_samples();
  // auto ms = smaep::data::map_source<double>::create_top_ten();

  for (const auto& e : expressions) {
    auto problem = std::get<0>(e);
    auto expected = std::get<1>(e);

    INFO(problem);
    auto ast = smaep::parse<double>(problem);
    // auto result = ast.value(ms);
    auto result = ast.value();

    CHECK(expected == result);
  }
}

// TEST_CASE("parse_to_int_ast", "parser") {
//   auto expressions = samples::get_int64_samples();
//   auto ms = smaep::data::map_source<int64_t>::create_top_ten();

//   for (const auto& e : expressions) {
//     auto problem = std::get<0>(e);
//     auto expected = std::get<1>(e);

//     INFO(problem);
//     auto ast = smaep::parse<int64_t>(problem);
//     auto result = ast.value(ms);

//     CHECK(expected == result);
//   }
// }

TEST_CASE("add_operator", "parser") {
  auto expressions =
      std::list<std::pair<std::string, int64_t>>{{"1 + 1", 2}, {"11 ~ 10", 10}};

  auto config = smaep::create_parser_config<int64_t>();
  config->add_operator("~", smaep::order(1),
                       [](auto x, auto i) { return (x / i) * i; });

  for (const auto& e : expressions) {
    auto problem = std::get<0>(e);
    auto expected = std::get<1>(e);

    INFO(problem);
    auto ast = smaep::parse<int64_t>(problem, config);
    auto result = ast.value();

    CHECK(expected == result);
  }
}
