#pragma once

#include <functional>
#include <iomanip>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>
#include <tuple>

#include "smaep/i_data_source.h"
#include "smaep/operators.hpp"

namespace smaep {

template <typename TValue> class abstract_visitor;

template <typename TValue> struct inode {
  virtual ~inode(){};
  virtual TValue value() = 0;
  virtual TValue value(const data::i_data_source<TValue> &data) = 0;

  virtual void accept_pre_order(abstract_visitor<TValue> &visitor) = 0;
  virtual void accept_post_order(abstract_visitor<TValue> &visitor) = 0;

  virtual std::ostream &write(std::ostream &out) const = 0;
};

template <typename TValue>
std::ostream &operator<<(std::ostream &out, const inode<TValue> &node) {
  return node.write(out);
}

template <typename TValue> class abstract_visitor {
public:
  ~abstract_visitor() = default;
  virtual void visit(const inode<TValue> *const node) = 0;
  virtual void push_parent(const inode<TValue> *const node){};
  virtual void pop_parent(){};
};

template <typename TValue> struct const_node : inode<TValue> {
  TValue data;

  const_node(TValue value) : data(value) {}
  ~const_node() {}

  TValue value() override { return data; }
  TValue value(const data::i_data_source<TValue> &) override { return data; }

  void accept_pre_order(abstract_visitor<TValue> &visitor) override {
    visitor.visit(this);
  };
  void accept_post_order(abstract_visitor<TValue> &visitor) override {
    visitor.visit(this);
  };

  std::ostream &write(std::ostream &out) const override {
    out << data;
    return out;
  }
};

template <typename TValue> struct var_node : inode<TValue> {
  std::string selector;

  var_node(const std::string &value) : selector(value) {}

  ~var_node() {}

  TValue value() override {
    throw std::domain_error("No data source available");
  }
  TValue value(const data::i_data_source<TValue> &data) override {
    return data.get_value_for(selector);
  }

  void accept_pre_order(abstract_visitor<TValue> &visitor) override {
    visitor.visit(this);
  };
  void accept_post_order(abstract_visitor<TValue> &visitor) override {
    visitor.visit(this);
  };

  std::ostream &write(std::ostream &out) const override {
    out << "\"" << selector << "\"";
    return out;
  }
};

template <typename TValue> struct i_function_node : inode<TValue> {
  virtual size_t size() const = 0;
  virtual void push_argument(std::unique_ptr<inode<TValue>> arg) = 0;
};

template <typename TValue, typename... TInputs>
struct function_node : i_function_node<TValue> {
private:
  const operation<TValue, TInputs...> &op;

  static const std::size_t N = sizeof...(TInputs);
  constexpr size_t size() const override { return N; }

  using node_ptr = std::unique_ptr<inode<TValue>>;
  using t_data = std::array<node_ptr, N>;
  t_data data = {};

  template <size_t... S> TValue call(std::index_sequence<S...>) {
    return op.function(std::get<S>(data)->value()...);
  }

  template <size_t... S>
  TValue call(std::index_sequence<S...>,
              const data::i_data_source<TValue> &source) {
    return op.function(std::get<S>(data)->value(source)...);
  }

public:
  void push_argument(std::unique_ptr<inode<TValue>> arg) override {
    for (size_t i = data.size() - 1; i >= 0; --i) {
      if (data[i] == nullptr) {
        data[i] = std::move(arg);
        return;
      }
    }
    throw std::logic_error("function arguments already satisfied");
  }

  function_node(const operation<TValue, TInputs...> &operation)
      : op(operation) {}

  ~function_node() {}

  TValue value() override {
    auto S = std::make_index_sequence<sizeof...(TInputs)>();
    return call(S);
  }
  TValue value(const data::i_data_source<TValue> &source) override {
    auto S = std::make_index_sequence<sizeof...(TInputs)>();
    return call(S, source);
  }

  void accept_pre_order(abstract_visitor<TValue> &visitor) override {
    visitor.visit(this);
    visitor.push_parent(this);
    for (node_ptr &node : data) {
      node->accept_pre_order(visitor);
    }
    visitor.pop_parent();
  };
  void accept_post_order(abstract_visitor<TValue> &visitor) override {
    visitor.push_parent(this);
    for (node_ptr &node : data) {
      node->accept_post_order(visitor);
    }
    visitor.pop_parent();
    visitor.visit(this);
  };

  std::ostream &write(std::ostream &out) const override {
    out << op.name;
    return out;
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

  void visit_pre_order(abstract_visitor<TValue> &visitor) {
    node->accept_pre_order(visitor);
  };
  void visit_post_order(abstract_visitor<TValue> &visitor) {
    node->accept_post_order(visitor);
  };
};

}; // namespace smaep
