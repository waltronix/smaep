#pragma once

#include <sstream>
#include <stack>
#include <string>

#include "smaep/tree.hpp"

namespace smaep {
template <typename TValue>
class postfix_printer : public smaep::abstract_visitor<TValue> {
private:
  std::stringstream m_stream;

public:
  void visit(const smaep::node_interface<TValue> *node) override {
    m_stream << " " << *node;
  }

  std::string str() { return m_stream.str().substr(1); }
};

template <typename TValue>
class ast_printer : public smaep::abstract_visitor<TValue> {
private:
  std::stringstream m_stream;
  std::stack<const smaep::node_interface<TValue> *> m_parents;

public:
  ast_printer() { m_stream << "ast" << std::endl; }

  void visit(const smaep::node_interface<TValue> *node) override {
    auto inc = std::string(2 * m_parents.size() + 2, ' ');
    m_stream << inc << "└ " << *node << std::endl;
  }
  void push_parent(const node_interface<TValue> *const node) override {
    m_parents.push(node);
  };
  void pop_parent() override { m_parents.pop(); };

  std::string str() { return m_stream.str(); }
};

} // namespace smaep
