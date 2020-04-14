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

TEST_CASE("Queries return correct result", "[basic]") {
  YAML::Node root = YAML::Load(
      "{city_001: {name: ab, x: 0, y: 0}, city_002: {name: abb, x: 0, y: 1}, "
      "city_003: {name: abbb, x: 0, y: 2}, city_004: {name: aabbb, x: 2, y : "
      "2}, city_005: {name: aaabbb, x: 3, y : 3} }");
  const auto map = makeMap(root);
  auto queryResult = query(map, {"aaabbb"});
  REQUIRE(queryResult.size() == 1);
  REQUIRE(queryResult.begin()->fromCity.name == "aaabbb");
  REQUIRE(queryResult.begin()->toCity.name == "NONE");

  queryResult = query(map, {"ab", "abb"});
  REQUIRE(queryResult.size() == 2);
  auto queryResultIt = queryResult.begin();
  REQUIRE(queryResultIt->fromCity.name == "ab");
  REQUIRE(queryResultIt->toCity.name == "abb");
  queryResultIt++;
  REQUIRE(queryResultIt->fromCity.name == "abb");
  REQUIRE(queryResultIt->toCity.name == "ab");

  queryResult = query(map, {"abbb", "aabbb"});
  queryResultIt = queryResult.begin();
  REQUIRE(queryResult.size() == 2);
  REQUIRE(queryResultIt->fromCity.name == "abbb");
  REQUIRE(queryResultIt->toCity.name == "aabbb");

  queryResultIt++;
  REQUIRE(queryResultIt->fromCity.name == "aabbb");
  REQUIRE(queryResultIt->toCity.name == "abbb");
}