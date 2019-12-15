#include <string>

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "json_source.h"

TEST_CASE("some_simple_jsonpath", "json_tests") {
  const std::string json = R"JSON(
{
  "x": "1",
  "y": "2"
}
)JSON";

  smaep::data::json_source<double> jw(json);

  auto rs = jw.get_value_for("$.x");
  REQUIRE(1 == rs);
}

TEST_CASE("nested jsonpath", "json_tests") {
  const std::string json = R"JSON(
{
  "A": {
    "x": "1",
    "y": "2"
  },
  "B": {
    "x": "3",
    "y": "4"
  }
}
)JSON";

  smaep::data::json_source<double> jw(json);

  REQUIRE(1 == jw.get_value_for("$.A.x"));
  REQUIRE(3 == jw.get_value_for("$.B.x"));
}

TEST_CASE("select temp", "json_tests") {
  const std::string json = R"JSON(
{
    "A": {
        "temp": "25.71",
        "pressure": "1013",
        "humidity": "53"
    }
}
)JSON";

  smaep::data::json_source<double> jw(json);

  auto temp = jw.get_value_for("$..temp");
  REQUIRE(25.71 == temp);
}

TEST_CASE("no_result", "json_tests") {
  const std::string json = R"JSON(
 {
   "y": "2"
 }
 )JSON";

  smaep::data::json_source<double> jw(json);

  REQUIRE_THROWS(jw.get_value_for("$.x"));
}

TEST_CASE("more_then_one_result", "json_tests") {
  const std::string json = R"JSON(
 {
   "A": {
     "x": "1"
   },
   "B": {
     "x": "2"
   }
 }
 )JSON";

  smaep::data::json_source<double> jw(json);

  REQUIRE_THROWS(jw.get_value_for("$.x"));
}
