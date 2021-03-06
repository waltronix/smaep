#include <filesystem>
#include <iostream>
#include <string>
#include <vector>


#include "CLI/CLI.hpp"

#include "json_source.h"
#include "smaep.h"
#include "smaep/visitors.hpp"

int main(int argc, char **argv) {
  CLI::App app{"smaep"};

  std::string expression;
  app.add_option("-e,--expression", expression,
                 "Expression that will be parsed and used to evaluate the "
                 "input (if any).");

  bool print_ast = false;
  app.add_flag("-t,--print_ast", print_ast,
               "Print the parsed tree after the result.");

  std::vector<std::string> json_files;
  app.add_option("-f,--files", json_files,
                 "JSON files that are used as data input.");

  CLI11_PARSE(app, argc, argv);

  try {
    auto ast = smaep::parse_double(expression);

    std::cout << std::endl << "  " << expression << std::endl << std::endl;

    if (print_ast) {
      smaep::ast_printer<double> printer;
      ast.visit_pre_order(printer);
      std::cout << std::endl << printer.str() << std::endl;
    }

    if (0 == json_files.size()) {
      auto result = ast.value();
      std::cout << expression << " = " << result << std::endl;
    } else {
      for (const auto &file : json_files) {
        std::cout << file << " : ";

        if (!std::filesystem::exists(file)) {
          std::cout << "file does not exist" << std::endl;
          continue;
        }

        std::ifstream ifs(file);
        std::ostringstream oss;
        oss << ifs.rdbuf();
        auto js = smaep::data::json_source<double>(oss.str());

        auto result = ast.value(js);
        std::cout << result << std::endl;
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "   Exception on: " << expression << std::endl;
    std::cerr << "     " << e.what() << std::endl;
  }

  return 0;
}