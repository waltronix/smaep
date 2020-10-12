#pragma once

#include <tao/pegtl.hpp>

// Include the analyze function that checks
// a grammar for possible infinite cycles.
#include <tao/pegtl/analyze.hpp>

#include "grammar.hpp"
#include "tree.hpp"
#include "tree_nursery.hpp"

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

namespace smaep {
template <typename TValue>
ast<TValue> parse(const std::string &expression,
                  std::shared_ptr<smaep::parser_config<TValue>> parser_config =
                      smaep::create_parser_config<TValue>()) {
  pegtl::analyze<smaep::grammar::grammar<TValue>>();

  pegtl::memory_input<> input(expression, "");
  smaep::tree_nursery<TValue> tree_nursery;

  try {
    pegtl::parse<grammar::grammar<TValue>, grammar::action>(
        input, parser_config, tree_nursery);
  } catch (const grammar::parse_error &e) {
    const auto p = e.positions.front();
    std::cerr << e.what() << std::endl
              << input.line_at(p) << std::endl
              << std::string(p.byte_in_line, ' ') << '^' << std::endl;
    throw;
  }

  return {parser_config, tree_nursery.get_ast()};
}
} // namespace smaep
