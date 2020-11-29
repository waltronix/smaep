#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include <math.h>

#include "catch2/catch.hpp"

#include "smaep.h"

template <typename TValue>
class printer : public smaep::abstract_visitor<TValue> {
private:
  std::stringstream m_stream;

public:
  void visit(const smaep::inode<TValue>* node) override {
    auto s = node->to_string();
    m_stream << s << " ";
  }

  std::string str() { return m_stream.str(); }
};

TEST_CASE("print_tree", "visitor") {

  auto expressions = std::list<std::tuple<std::string, double>>();
  expressions.push_back({"(1 + 1) * 2", 4});
  expressions.push_back({"2 / (1 + 1)", 1});
  expressions.push_back({"23 * 2 - 4", 42});
  expressions.push_back({"sin(1/3)^2 + cos(1/3)^2", 1});

  for (const auto &[problem, expected] : expressions) {
    INFO(problem);
    auto ast = smaep::parse_double(problem);

    printer<double> p;
    ast.node->accept(p);
    auto str = p.str();
    std::cout << str << std::endl;
  }
}