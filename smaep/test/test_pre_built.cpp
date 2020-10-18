#include <list>
#include <string>

#include <math.h>

#include "catch2/catch.hpp"

#include "smaep.h"

TEST_CASE("pre built double parser", "[pre built]") {
    auto ast = smaep::parse_double("1 + 1");
    auto result = ast.value();

    CHECK(2 == result);
}

TEST_CASE("pre built int64_t parser", "[pre built]") {
    auto ast = smaep::parse_int64_t("1 + 1");
    auto result = ast.value();

    CHECK(2 == result);
}