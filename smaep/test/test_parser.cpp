#include <list>
#include <string>

#include <math.h>

#include "catch2/catch.hpp"

#include "smaep.h"

TEST_CASE("parse_numbers", "parser") {

  auto expressions = std::list<std::tuple<std::string, double>>();
  expressions.push_back({"0", 0});
  expressions.push_back({"1", 1});
  expressions.push_back({"2", 2});
  expressions.push_back({"3", 3});
  expressions.push_back({"4", 4});
  expressions.push_back({"5", 5});
  expressions.push_back({"6", 6});
  expressions.push_back({"7", 7});
  expressions.push_back({"8", 8});
  expressions.push_back({"9", 9});

  expressions.push_back({"-1", -1});
  expressions.push_back({"-5", -5});

  expressions.push_back({"10", 10});
  expressions.push_back({"222", 222});
  expressions.push_back({"-222", -222});

  for (const auto &[problem, expected] : expressions) {
    INFO(problem);
    auto ast = smaep::parse_double(problem);
    // auto result = ast.value(ms);
    auto result = ast.value();

    CHECK(expected == result);
  }
}

TEST_CASE("parsing fails", "parser") {
  auto expressions = std::list<std::string>();
  expressions.push_back("+");

  auto config = smaep::parser_config<double>::create();

  for (const auto &problem : expressions) {

    INFO(problem);
    CHECK_THROWS(smaep::parse_double(problem, config));
  }
}

TEST_CASE("primitive_operations", "parser") {

  auto expressions = std::list<std::tuple<std::string, double>>();
  expressions.push_back({"1 + 1", 2});
  expressions.push_back({"2 - 1", 1});
  expressions.push_back({"2 * 2", 4});
  expressions.push_back({"6 / 2", 3});
  expressions.push_back({"2^3", 8});

  for (const auto &[problem, expected] : expressions) {
    INFO(problem);
    auto ast = smaep::parse_double(problem);
    // auto result = ast.value(ms);
    auto result = ast.value();

    CHECK(expected == result);
  }
}

TEST_CASE("function_operations", "parser") {

  auto expressions = std::list<std::tuple<std::string, double>>();
  expressions.push_back({"abs(2)", 2});
  expressions.push_back({"abs(-2)", 2});

  expressions.push_back({"exp(0)", 1});
  expressions.push_back({"exp(1)", exp(1.)});

  expressions.push_back({"sin(0)", 0});
  expressions.push_back({"sin(30)", sin(30.)});

  expressions.push_back({"cos(0)", 1});
  expressions.push_back({"cos(30)", cos(30.)});

  expressions.push_back({"tan(0)", 0});
  expressions.push_back({"tan(30)", tan(30.)});

  for (const auto &[problem, expected] : expressions) {
    INFO(problem);
    auto ast = smaep::parse_double(problem);
    // auto result = ast.value(ms);
    auto result = ast.value();

    CHECK(expected == result);
  }
}

TEST_CASE("braced_expressions", "parser") {

  auto expressions = std::list<std::tuple<std::string, double>>();
  expressions.push_back({"(1 + 1) * 2", 4});
  expressions.push_back({"2 / (1 + 1)", 1});
  expressions.push_back({"sin(1/3)^2 + cos(1/3)^2", 1});

  for (const auto &[problem, expected] : expressions) {
    INFO(problem);
    auto ast = smaep::parse_double(problem);
    // auto result = ast.value(ms);
    auto result = ast.value();

    CHECK(expected == result);
  }
}

TEST_CASE("user_defined_operator", "parser") {
  auto expressions = std::list<std::pair<std::string, int64_t>>();
  expressions.push_back({"1 + 1", 2});
  expressions.push_back({"11 ~ 10", 10});
  expressions.push_back({"11 || 10", 1});

  auto config = smaep::parser_config<int64_t>::create();
  config->add_operator("~", smaep::order(1),
                       [](auto x, auto i) { return (x / i) * i; });

  config->add_operator("||", smaep::order(1), [](auto x, auto i) { return 1; });

  for (const auto &[problem, expected] : expressions) {
    INFO(problem);
    auto ast = smaep::parse_int64_t(problem, config);
    auto result = ast.value();

    CHECK(expected == result);
  }
}

TEST_CASE("user_defined_function", "parser") {
  auto expressions = std::list<std::pair<std::string, int64_t>>();
  expressions.push_back({"my_func(1)", 5});
  expressions.push_back({"my_func(3)", 5});

  auto config = smaep::parser_config<int64_t>::create();
  config->add_function("my_func", smaep::order(1), [](auto x) { return 5; });

  for (const auto &[problem, expected] : expressions) {
    INFO(problem);
    auto ast = smaep::parse_int64_t(problem, config);
    auto result = ast.value();

    CHECK(expected == result);
  }
}
