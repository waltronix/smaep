#include <iostream>
#include <list>
#include <string>

#include "catch2/catch.hpp"

#include "smaep.h"
#include "smaep/data_source_samples.hpp"
#include "smaep/visitors.hpp"

TEST_CASE("print expression", "postfix_printer") {

  auto expressions = std::list<std::tuple<std::string, std::string>>();
  expressions.push_back({"(1.5 + 1) * 2", "1.5 1 + 2 *"});
  expressions.push_back({"2 / (1 + 1)", "2 1 1 + /"});
  expressions.push_back({"23 * 2 - 4", "23 2 * 4 -"});
  expressions.push_back(
      {"sin(1/3)^2 + cos(1/3)^2", "1 3 / sin 2 ^ 1 3 / cos 2 ^ +"});

  for (const auto &[problem, expected_upn] : expressions) {
    INFO(problem);
    auto ast = smaep::parse_double(problem);

    smaep::postfix_printer<double> p;
    ast.visit_post_order(p);
    auto result = p.str();

    CHECK(expected_upn == result);
  }
}

TEST_CASE("print ast", "ast_printer") {

  const std::string expected_ast = R"xx(ast
  └ +
    └ -1
    └ abs
      └ -
        └ "A"
        └ 3.14
)xx";

  auto problem = "-1 + abs(data[A]-3.14)";
  auto ast = smaep::parse_double(problem);

  smaep::ast_printer<double> ap;
  ast.visit_pre_order(ap);
  auto result = ap.str();

  CHECK(expected_ast == result);
}
