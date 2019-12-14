#pragma once

#include <list>
#include <string>
#include <tuple>

namespace samples {
auto get_double_samples() {
  auto expressions = std::list<std::tuple<std::string, double>>();
  // expressions.push_back({"1", 1 });
  expressions.push_back({"1 + 1", 2});
  expressions.push_back({"1 * 1", 1});
  expressions.push_back({"(1 * 1)", 1});
  expressions.push_back({"2 * (3 + 1)", 8});
  expressions.push_back({"2 * 3 + 1", 7});
  expressions.push_back({"1 * 2 + 3", 5});
  expressions.push_back({"1 + 2 * 3", 7});
  expressions.push_back({"data[one]", 1});
  expressions.push_back({"data[one] + 2", 3});
  expressions.push_back({"data[two] + data[three]", 5});
  expressions.push_back({"2 * (data[one] + 1)", 4});
  expressions.push_back({"3 * (1 + 2 * (data[one] + 1))", 15});
  expressions.push_back({"3 * (1 + 2 * (data[four] + 1))", 33});
  expressions.push_back({"1 / 2", 0.5});
  expressions.push_back({"1.5 / 2", 0.75});
  expressions.push_back({"abs(-2)", 2});
  expressions.push_back({"sin(1/3)^2 + cos(1/3)^2", 1});

  return expressions;
}

auto get_int64_samples() {
  auto expressions = std::list<std::tuple<std::string, int64_t>>();
  expressions.push_back({"1 + 1", 2});
  expressions.push_back({"1 * 1", 1});
  expressions.push_back({"(1 * 1)", 1});
  expressions.push_back({"2 * (3 + 1)", 8});
  expressions.push_back({"2 * 3 + 1", 7});
  expressions.push_back({"1 * 2 + 3", 5});
  expressions.push_back({"1 + 2 * 3", 7});
  expressions.push_back({"data[one]", 1});
  expressions.push_back({"data[one] + 2", 3});
  expressions.push_back({"data[two] + data[three]", 5});
  expressions.push_back({"2 * (data[one] + 1)", 4});
  expressions.push_back({"3 * (1 + 2 * (data[one] + 1))", 15});
  expressions.push_back({"3 * (1 + 2 * (data[four] + 1))", 33});
  expressions.push_back({"1 / 2", 0});
  // expressions.push_back({ "1 << 2", 4 });

  return expressions;
}
}  // namespace samples
