#pragma once

// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <cassert>
#include <iostream>
#include <sstream>

#include <tao/pegtl.hpp>
namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

#include "i_tree_nursery.h"
#include "operators.hpp"

namespace smaep::grammar {
// Here the actual grammar starts.
using namespace tao::pegtl;  // NOLINT

// Comments are introduced by a '#' and proceed to the end-of-line/file.
struct comment : if_must<one<'#'>, until<eolf>> {};

// The calculator ignores all spaces and comments; space is a pegtl rule
// that matches the usual ascii characters ' ', '\t', '\n' etc. In other
// words, everything that is space or a comment is ignored.
struct ignored : sor<space, comment> {};

// Since the binary operators are taken from a runtime data structure
// (rather than hard-coding them into the grammar), we need a custom
// rule that attempts to match the input against the current map of
// operators.
template <typename TValue>
struct infix {
  using analyze_t = analysis::generic<analysis::rule_type::any>;

  template <apply_mode,
            rewind_mode,
            template <typename...>
            class Action,
            template <typename...>
            class Control,
            typename Input,
            typename... States>
  static bool match(Input& in,
                    const std::shared_ptr<smaep::parser_config<TValue>>& b,
                    States&&... /*unused*/) {
    // Look for the longest match of the input against the operators in the
    // operator map.
    return match(in, b, std::string());
  }

 private:
  template <typename Input>
  static bool match(
      Input& in,
      const std::shared_ptr<smaep::parser_config<TValue>>& all_operators,
      std::string match_string) {
    // ToDo Check if recursion is really necessary here.
    if (in.size(match_string.size() + 1) > match_string.size()) {
      match_string += in.peek_char(match_string.size());
      const auto& [first_match, after_match] =
          all_operators->operators().equal_range(match_string);
      if (first_match != all_operators->operators().end()) {
        if (match(in, all_operators, match_string)) {
          return true;
        }
        if (first_match->first == match_string) {
          in.bump(match_string.size());
          return true;
        }
      }
    }
    return false;
  }
};

template <typename TNumber>
struct number {};

template <>
struct number<int64_t> : seq<opt<one<'+', '-'>>, plus<digit>> {};

template <>
struct number<double>
    : seq<opt<one<'+', '-'>>, plus<digit>, opt<one<'.'>, star<digit>>> {};

template <typename TNumber>
struct expression;

template <typename TNumber>
struct parentheses : if_must<one<'('>, expression<TNumber>, one<')'>> {};

struct data_key : plus<not_one<']'>> {};

struct data_variable
    : if_must<string<'d', 'a', 't', 'a', '['>, data_key, one<']'>> {};

struct function_name : identifier {};

template <typename TNumber>
struct function : if_must<function_name, parentheses<TNumber>> {};

// An atomic expression, i.e. one without operators, is either a number or
// a bracketed expression.
template <typename TNumber>
struct atomic : sor<number<TNumber>,
                    parentheses<TNumber>,
                    data_variable,
                    function<TNumber>> {};

// An expression is a non-empty list of atomic expressions where each pair
// of atomic expressions is separated by an infix operator and we allow
// the rule ignored as padding (before and after every singlar expression).
template <typename TNumber>
struct expression : list<atomic<TNumber>, infix<TNumber>, ignored> {};

// The top-level grammar allows one expression and then expects eof.
template <typename TNumber>
struct grammar : must<expression<TNumber>, eof> {};

// After the grammar we proceed with the additional actions that are
// required to let our calculator actually do something.
template <typename Rule>
struct action {};

// This action will be called when the number rule matches; it converts the
// matched portion of the input to a double and pushes it onto the operand
// tree_nursery.
template <typename TNumber>
struct action<number<TNumber>> {
  template <typename Input>
  static void apply(
      const Input& in,
      const std::shared_ptr<smaep::parser_config<TNumber>>& /*unused*/,
      i_tree_nursery<TNumber>& s) {
    std::stringstream ss(in.string());
    TNumber v;
    ss >> v;
    s.push_operand(v);
  }
};

template <>
struct action<data_key> {
  template <typename Input, typename TNumber>
  static void apply(
      const Input& in,
      const std::shared_ptr<smaep::parser_config<TNumber>>& /*unused*/,
      i_tree_nursery<TNumber>& s) {
    s.push_operand(in.string());
  }
};

template <typename TNumber>
struct action<infix<TNumber>> {
  template <typename Input>
  static void apply(const Input& in,
                    const std::shared_ptr<smaep::parser_config<TNumber>>& ops,
                    i_tree_nursery<TNumber>& s) {
    auto op_name = in.string();
    auto& op = ops->operators().at(op_name);
    s.push_operation(op);
  }
};

template <>
struct action<function_name> {
  template <typename Input, typename TNumber>
  static void apply(const Input& in,
                    const std::shared_ptr<smaep::parser_config<TNumber>>& ops,
                    i_tree_nursery<TNumber>& s) {
    auto op_name = in.string();
    auto& op = ops->functions().at(op_name);
    s.push_function(op);
  }
};

template <>
struct action<one<'('>> {
  template <typename TNumber>
  static void apply0(const std::shared_ptr<smaep::parser_config<TNumber>>& ops,
                     i_tree_nursery<TNumber>& s) {
    s.open_parentheses();
  }
};

template <>
struct action<one<')'>> {
  template <typename TNumber>
  static void apply0(
      const std::shared_ptr<smaep::parser_config<TNumber>>& /*unused*/,
      i_tree_nursery<TNumber>& s) {
    s.close_parentheses();
  }
};
}
