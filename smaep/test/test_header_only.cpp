#include <list>
#include <string>

#include <math.h>

#include "catch2/catch.hpp"

#include "smaep/parser.hpp"

TEST_CASE("header only double parser", "[header only]") {
    auto ast = smaep::parse<double>("1 + 1");
    auto result = ast.value();

    CHECK(2 == result);
}

TEST_CASE("header only int64_t parser", "[header only]") {
    auto ast = smaep::parse<int64_t>("1 + 1");
    auto result = ast.value();

    CHECK(2 == result);
}