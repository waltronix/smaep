#pragma once

#include <functional>
#include <iomanip>
#include <memory>
#include <ostream>
#include <string>
#include <tuple>

#include "smaep/i_data_source.h"
#include "smaep/operators.hpp"

namespace smaep {
template <typename TValue> struct inode {
  virtual ~inode(){};
  virtual TValue value() = 0;
  virtual TValue value(const data::i_data_source<TValue> &data) = 0;

  virtual std::string to_string() = 0;

  virtual void to_dot(std::ostream &dot, uint32_t inc = 0) {
    dot << std::string(2 * inc, ' ') << " └ " << std::left << to_string()
        << std::endl;
  }
};

template <typename TValue> struct const_node : inode<TValue> {
  TValue data;

  const_node(TValue value) : data(value) {}

  ~const_node() {}

  TValue value() final { return data; }

  TValue value(const data::i_data_source<TValue> &) final { return data; }

  std::string to_string() override { return std::to_string(data); }
};

template <typename TValue> struct var_node : inode<TValue> {
  std::string selector;

  var_node(const std::string &value) : selector(value) {}

  ~var_node() {}

  TValue value() final { throw std::domain_error("No data source available"); }

  TValue value(const data::i_data_source<TValue> &data) override {
    return data.get_value_for(selector);
  }

  std::string to_string() override { return "\"" + selector + "\""; }
};

template <typename TValue> struct i_function_node : inode<TValue> {
  virtual size_t num_arguments() = 0;
  virtual void push_argument(std::unique_ptr<inode<TValue>> arg) = 0;
};

template <typename TValue, typename... TInputs>
struct function_node : i_function_node<TValue> {
private:
  const operation<TValue, TInputs...> &op;

  using t_data = std::tuple<std::unique_ptr<inode<TInputs>>...>;
  t_data data = {};

  template <size_t I = sizeof...(TInputs) - 1, typename TInput>
  void static push_arg(t_data &data, std::unique_ptr<inode<TInput>> arg) {
    if (std::get<I>(data) == nullptr) {
      std::get<I>(data) = std::move(arg);
    } else {
      if constexpr (I == 0) {
        throw std::logic_error("function arguments already satisfied");
      } else {
        push_arg<I - 1>(data, std::move(arg));
      }
    }
  }

  template <size_t... S> TValue call(std::index_sequence<S...>) {
    return op.function(std::get<S>(data)->value()...);
  }

  template <size_t... S>
  TValue call(std::index_sequence<S...>,
              const data::i_data_source<TValue> &source) {
    return op.function(std::get<S>(data)->value(source)...);
  }

public:
  size_t num_arguments() final { return sizeof...(TInputs); }

  void push_argument(std::unique_ptr<inode<TValue>> arg) final {
    push_arg(data, std::move(arg));
  }

  function_node(const operation<TValue, TInputs...> &operation)
      : op(operation) {}

  ~function_node() {}

  std::string to_string() override {

    std::string postfix = (num_arguments() == 1) ? "()" : "";
    return op.name + postfix;
  }

  TValue value() final {
    auto S = std::make_index_sequence<sizeof...(TInputs)>();
    return call(S);
  }

  TValue value(const data::i_data_source<TValue> &source) final {
    auto S = std::make_index_sequence<sizeof...(TInputs)>();
    return call(S, source);
  }

  void to_dot(std::ostream &dot, uint32_t inc = 0) override {
    i_function_node<TValue>::to_dot(dot, inc);
    std::apply(
        [&dot, inc](const auto &... x) { (..., x->to_dot(dot, inc + 1)); },
        data);
  }
};

template <typename TOut, typename... TIn>
std::unique_ptr<function_node<TOut, TIn...>>
make_node(const operation<TOut, TIn...> &op) {
  return std::make_unique<function_node<TOut, TIn...>>(op);
}

template <typename TValue> struct ast {
  const std::shared_ptr<parser_config<TValue>> operations;
  const std::unique_ptr<inode<TValue>> node;

  TValue value() { return node->value(); }

  TValue value(const data::i_data_source<TValue> &data) {
    return node->value(data);
  }

  std::string to_dot() {
    std::stringstream dot;
    dot << "ast" << std::endl;
    node->to_dot(dot, 1);

    return dot.str();
  }
};

}; // namespace smaep
