#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include <math.h>

#include "catch2/catch.hpp"

#include "smaep.h"

template <typename TValue>
class upn_printer : public smaep::abstract_visitor<TValue> {
private:
  std::stringstream m_stream;

public:
  void visit(const smaep::inode<TValue> *node) override {
    m_stream << " " << *node;
  }

  std::string str() { return m_stream.str().substr(1); }
};

TEST_CASE("print_tree", "visitor") {

  auto expressions = std::list<std::tuple<std::string, std::string>>();
  expressions.push_back({"(1.5 + 1) * 2", "1.5 1 + 2 *"});
  expressions.push_back({"2 / (1 + 1)", "2 1 1 + /"});
  expressions.push_back({"23 * 2 - 4", "23 2 * 4 -"});
  expressions.push_back(
      {"sin(1/3)^2 + cos(1/3)^2", "1 3 / sin() 2 ^ 1 3 / cos() 2 ^ +"});

  for (const auto &[problem, expected_upn] : expressions) {
    INFO(problem);
    auto ast = smaep::parse_double(problem);

    upn_printer<double> p;
    ast.node->accept(p);
    auto result = p.str();

    CHECK(expected_upn == result);
  }
}