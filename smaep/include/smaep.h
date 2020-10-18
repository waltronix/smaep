#pragma once

#include "smaep/operators.hpp"
#include "smaep/tree.hpp"

namespace smaep {
ast<double> parse_double(
    const std::string& expression,
    std::shared_ptr<smaep::parser_config<double>> parser_config =
        smaep::parser_config<double>::create());

ast<int64_t> parse_int64_t(
    const std::string& expression,
    std::shared_ptr<smaep::parser_config<int64_t>> parser_config =
        smaep::parser_config<int64_t>::create());
}  // namespace smaep