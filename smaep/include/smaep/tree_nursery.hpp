#pragma once

#include <memory>
#include <stack>

#include "smaep/tree_nursery_interface.h"

#include "smaep/operators.hpp"
#include "smaep/tree.hpp"

namespace smaep {
/** Class that takes care of an operand and an operator stack for shift-reduce
 *  style handling of operator priority
 */
template <typename TValue> class tree_nursery : public tree_nursery_interface<TValue> {
public:
  tree_nursery() {
    // we initialize the operator stack with an opening parenthesis, so we can
    // be sure there is always an element and do not always have to check
    open_parentheses();
  }

  void push_operand(std::unique_ptr<node_interface<TValue>> node) override {
    m_operands.push(std::move(node));
  }

  void push_operand(const TValue value) override {
    m_operands.push(std::make_unique<const_node<TValue>>(value));
  }

  void push_operand(const std::string &s) override {
    m_operands.push(std::make_unique<var_node<TValue>>(s));
  }

  /** When pushing operations, we have to take care of operator precedence. */
  void
  push_operation(const operation<TValue, TValue, TValue> &operation) override {
    while (m_operations.top().first <= operation.precedence) {
      // new operator has higher precedence than last one
      apply_one_operation();
    }
    m_operations.push({operation.precedence, make_node(operation)});
  }

  void push_function(const operation<TValue, TValue> &function) override {
    m_operations.push({function.precedence, make_node(function)});
  }

  void open_parentheses() override {
    m_operations.push({order::parens, nullptr});
  }

  void close_parentheses() override {
    std::unique_ptr<node_interface<TValue>> r = make_subtree();
    m_operands.push(std::move(r));
  }

  std::unique_ptr<node_interface<TValue>> get_ast() override { return make_subtree(); }

private:
  std::stack<std::unique_ptr<node_interface<TValue>>> m_operands;
  std::stack<std::pair<order, std::unique_ptr<function_node_interface<TValue>>>>
      m_operations;

  /** Applies stack operations until an 'opening' parentheses is found. */
  std::unique_ptr<node_interface<TValue>> make_subtree() {
    while (m_operations.top().first != order::parens) {
      apply_one_operation();
    }
    // now remove the opening parenthesis
    m_operations.pop();

    std::unique_ptr<node_interface<TValue>> r;
    m_operands.top().swap(r);
    m_operands.pop();

    return std::move(r);
  }

  /** Get one operation from the operator stack, fill it with arguments and push
   * it to the operand stack.
   */
  void apply_one_operation() {
    std::unique_ptr<function_node_interface<TValue>> operation =
        std::move(m_operations.top().second);
    m_operations.top().second.release();
    m_operations.pop();

    auto num_args = operation->size();
    for (auto i = 0; i < num_args; ++i) {
      operation->push_argument(std::move(m_operands.top()));
      m_operands.pop();
    }

    // now the operation has its arguments applied and can be stored like an
    // operand
    push_operand(std::move(operation));
  }
};
} // namespace smaep
