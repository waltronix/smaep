#include <iostream>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include "map_source.h"
#include "parser.hpp"

int main(int argc, char** argv) {
  std::vector<std::string> args(argv, argv + argc);

  bool print_ast = std::any_of(args.cbegin(), args.cend(),
                               [](const auto& x) -> bool { return x == "-a"; });

  if (args.size() == 1) {
    std::cout << "expression required" << std::endl;
  } else {
    args.erase(args.begin());
    for (const auto& expression : args) {
      if ("-a" == expression) {
        continue;
      }
      try {
        auto ms = smaep::data::map_source<double>::create_top_ten();

        auto ast = smaep::parse<double>(expression);
        auto result = ast.value(ms);

        std::cout << std::endl
                  << "  " << expression << " = " << result << std::endl;

        if (print_ast) {
          std::cout << std::endl << ast.to_dot() << std::endl;
        }
      } catch (const std::exception& e) {
        std::cerr << "   Exception on: " << expression << std::endl;
        std::cerr << "     " << e.what() << std::endl;
        ;
      }
    }
  }

  return 0;
}