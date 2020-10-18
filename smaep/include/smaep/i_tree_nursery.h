#pragma once

#include <memory>
#include <string>

#include "smaep/operators.hpp"
#include "smaep/tree.hpp"

namespace smaep {
template <typename TValue>
class i_tree_nursery {
 public:
  virtual void push_operand(std::unique_ptr<inode<TValue>> node) = 0;
  virtual void push_operand(const TValue value) = 0;
  virtual void push_operand(const std::string& s) = 0;

  virtual void push_operation(
      const operation<TValue, TValue, TValue>& operation) = 0;
  virtual void push_function(const operation<TValue, TValue>& function) = 0;

  virtual void open_parentheses() = 0;
  virtual void close_parentheses() = 0;

  virtual std::unique_ptr<inode<TValue>> get_ast() = 0;
};
}  // namespace smaep
