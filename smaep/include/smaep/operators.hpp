#pragma once

// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <cassert>
#include <cmath>
#include <functional>
#include <limits>
#include <map>
#include <memory>

namespace smaep {
// This enum is used for the order in which the operators are
// evaluated, i.e. the priority of the operators; a higher
// number indicates a lower priority.
enum class order : uint8_t {
  min = 0,
  parens = std::numeric_limits<uint8_t>::max(),
  max = std::numeric_limits<uint8_t>::max()
};

template <typename TValue, typename... TInputs>
struct operation {
  std::string name;
  order precedence;
  std::function<TValue(TInputs...)> function;
};

template <typename TValue>
class parser_config {
 private:
  std::map<std::string, operation<TValue, TValue, TValue>> m_infix_operators;
  std::map<std::string, operation<TValue, TValue>> m_functions;

  static std::shared_ptr<parser_config<TValue>> create_default() {
    auto config = std::make_shared<parser_config<TValue>>();

    config->add_operator("^", order(4), [](const TValue l, const TValue r) {
      return pow(l, r);
    });
    config->add_operator("*", order(5),
                         [](const TValue l, const TValue r) { return l * r; });
    config->add_operator("/", order(5),
                         [](const TValue l, const TValue r) { return l / r; });
    config->add_operator("+", order(6),
                         [](const TValue l, const TValue r) { return l + r; });
    config->add_operator("-", order(6),
                         [](const TValue l, const TValue r) { return l - r; });

    config->add_function("abs", order(1),
                         [](const TValue x) { return std::abs(x); });

    return config;
  }

 public:
  void add_operator(const std::string& name,
                    const order precedence,
                    const std::function<TValue(TValue, TValue)>& operation) {
    m_infix_operators.insert({name, {name, precedence, operation}});
  }

  void add_function(const std::string& name,
                    const order precedence,
                    const std::function<TValue(TValue)>& function) {
    m_functions.insert({name, {name, precedence, function}});
  }

  const auto& operators() const { return m_infix_operators; }

  const auto& functions() const { return m_functions; }

  static std::shared_ptr<parser_config<TValue>> create() {
    return create_default();
  }
};

template <>
inline std::shared_ptr<parser_config<double>> parser_config<double>::create() {
  auto config = parser_config<double>::create_default();

   config->add_function("exp", order(1),
                       [](const double x) { return std::exp(x); });

   config->add_function("sin", order(1),
                       [](const double x) { return std::sin(x); });
   config->add_function("cos", order(1),
                       [](const double x) { return std::cos(x); });
   config->add_function("tan", order(1),
                       [](const double x) { return std::tan(x); });

  return config;
}

}  // namespace smaep
