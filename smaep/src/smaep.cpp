#include "smaep.h"

#include "smaep/parser.hpp"

namespace smaep {
ast<double>
parse_double(const std::string &expression,
             std::shared_ptr<smaep::parser_config<double>> parser_config) {
  return smaep::parse<double>(expression, parser_config);
}

ast<int64_t>
parse_int64_t(const std::string &expression,
              std::shared_ptr<smaep::parser_config<int64_t>> parser_config) {
  return smaep::parse<int64_t>(expression, parser_config);
}
} // namespace smaep
