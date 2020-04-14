#include "city.h"

#include <catch2/catch.hpp>
#include <yaml-cpp/yaml.h>

#include <limits>

TEST_CASE("Verify Location defaults", "[basic]") {
  const Location location;
  REQUIRE(location.x == std::numeric_limits<int>::max());
  REQUIRE(location.y == std::numeric_limits<int>::max());
}

TEST_CASE("Verify City defaults", "[basic]") {
  const City city;
  REQUIRE(city.name == "NONE");
  REQUIRE(city.insertionIndex == 0);
  REQUIRE(city.coordinates.x == std::numeric_limits<int>::max());
  REQUIRE(city.coordinates.y == std::numeric_limits<int>::max());
}

TEST_CASE("Require empty input creates empty close city map", "[basic]") {
  YAML::Node root = YAML::Load("");
  const auto map = makeMap(root);
  REQUIRE(map.empty());
}
