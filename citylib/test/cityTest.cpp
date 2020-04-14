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

TEST_CASE("Distance returns correct value", "[basic]") {
  const auto cityA = City{"A", {0, 0}, 0};
  const auto cityB = City{"B", {0, 1}, 1};
  const auto cityC = City{"C", {1, 1}, 2};
  const auto cityD = City{"D", {1, 2}, 2};

  SECTION("same city") {
    REQUIRE(distance(cityA, cityA) == 0);
    REQUIRE(distance(cityB, cityB) == 0);
    REQUIRE(distance(cityC, cityC) == 0);
  }
  SECTION("differtent cities") {
    REQUIRE(distance(cityA, cityB) == 1);
    REQUIRE(distance(cityA, cityC) == 2);
    REQUIRE(distance(cityA, cityD) == 3);
    REQUIRE(distance(cityB, cityC) == 1);
  }
}

TEST_CASE("Empty input creates empty close city map", "[basic]") {
  YAML::Node root = YAML::Load("");
  const auto map = makeMap(root);
  REQUIRE(map.empty());
}

TEST_CASE("Query on empty map throws", "[basic]") {
  YAML::Node root = YAML::Load("");
  const auto emptyMap = makeMap(root);
  REQUIRE_THROWS(query(emptyMap, {"a"}));
}

TEST_CASE("Query for non-existing citythrows", "[basic]") {
  YAML::Node root = YAML::Load("{city_001: {name: ab, x: 0, y: 0}}");
  const auto map = makeMap(root);
  REQUIRE_THROWS(query(map, {"b"}));
}