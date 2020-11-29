#pragma once

#include <sstream>
#include <string>

#include "smaep/tree.hpp"

namespace smaep {
template <typename TValue>
class postfix_printer : public smaep::abstract_visitor<TValue> {
private:
  std::stringstream m_stream;

public:
  void visit(const smaep::inode<TValue> *node) override {
    m_stream << " " << *node;
  }

  std::string str() { return m_stream.str().substr(1); }
};

template <typename TValue>
class ast_printer : public smaep::abstract_visitor<TValue> {
private:
  std::stringstream m_stream;

public:
  void visit(const smaep::inode<TValue> *node) override {
    m_stream << " " << *node;
  }

  std::string str() { return m_stream.str().substr(1); }
};

} // namespace smaep
