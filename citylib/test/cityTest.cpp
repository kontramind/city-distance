#include "city.h"

#include <catch2/catch.hpp>
#include <yaml-cpp/yaml.h>

#include <limits>

using namespace Cities;

TEST_CASE("Verify Location defaults", "[basic]") {
  const Location location{0, 0};
  REQUIRE(location.x() == 0);
  REQUIRE(location.y() == 0);
}

TEST_CASE("Verify City defaults", "[basic]") {
  const City city{"a", {0, 0}};
  REQUIRE(city.name() == "a");
  REQUIRE(city.location().x() == 0);
  REQUIRE(city.location().y() == 0);
}

TEST_CASE("Distance returns correct value", "[basic]") {
  const auto cityA = City{"A", {0, 0}};
  const auto cityB = City{"B", {0, 1}};
  const auto cityC = City{"C", {1, 1}};
  const auto cityD = City{"D", {1, 2}};

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

TEST_CASE("One city", "[basic]") {
  YAML::Node root = YAML::Load("{city_001: {name: ab, x: 0, y: 0}}");
  const auto map = makeMap(root);
  auto queryResult = query(map, {"ab"});
  auto queryResultIt = queryResult.begin();
  REQUIRE(queryResultIt->from().name() == "ab");
  REQUIRE_FALSE(queryResultIt->to());
}

TEST_CASE("Two disconnected cities", "[basic]") {
  YAML::Node root = YAML::Load(
      "{city_001: {name: ab, x: 0, y: 0}, city_002: {name: cd, x: 2, y: 2}}");
  const auto map = makeMap(root);

  auto queryResult = query(map, {"ab"});
  REQUIRE(queryResult.size() == 1);
  auto queryResultIt = queryResult.begin();
  REQUIRE(queryResultIt->from().name() == "ab");
  REQUIRE_FALSE(queryResultIt->to());

  queryResult = query(map, {"cd"});
  REQUIRE(queryResult.size() == 1);
  queryResultIt = queryResult.begin();
  REQUIRE(queryResultIt->from().name() == "cd");
  REQUIRE_FALSE(queryResultIt->to());

  queryResult = query(map, {"ab", "cd"});
  REQUIRE(queryResult.size() == 2);
  queryResultIt = queryResult.begin();
  REQUIRE(queryResultIt->from().name() == "ab");
  REQUIRE_FALSE(queryResultIt->to());
  queryResultIt++;
  REQUIRE(queryResultIt->from().name() == "cd");
  REQUIRE_FALSE(queryResultIt->to());

  queryResult = query(map, {"cd", "ab"});
  REQUIRE(queryResult.size() == 2);
  queryResultIt = queryResult.begin();
  REQUIRE(queryResultIt->from().name() == "cd");
  REQUIRE_FALSE(queryResultIt->to());
  queryResultIt++;
  REQUIRE(queryResultIt->from().name() == "ab");
  REQUIRE_FALSE(queryResultIt->to());
}

TEST_CASE("Two connected cities", "[basic]") {
  YAML::Node root = YAML::Load(
      "{city_001: {name: ab, x: 0, y: 0}, city_002: {name: cd, x: 1, y: 0}}");
  const auto map = makeMap(root);

  auto queryResult = query(map, {"ab"});
  REQUIRE(queryResult.size() == 1);
  auto queryResultIt = queryResult.begin();
  REQUIRE(queryResultIt->from().name() == "ab");
  REQUIRE(queryResultIt->to()->name() == "cd");

  queryResult = query(map, {"cd"});
  REQUIRE(queryResult.size() == 1);
  queryResultIt = queryResult.begin();
  REQUIRE(queryResultIt->from().name() == "cd");
  REQUIRE(queryResultIt->to()->name() == "ab");

  queryResult = query(map, {"ab", "cd"});
  REQUIRE(queryResult.size() == 2);
  queryResultIt = queryResult.begin();
  REQUIRE(queryResultIt->from().name() == "ab");
  REQUIRE(queryResultIt->to()->name() == "cd");
  queryResultIt++;
  REQUIRE(queryResultIt->from().name() == "cd");
  REQUIRE(queryResultIt->to()->name() == "ab");

  queryResult = query(map, {"cd", "ab"});
  REQUIRE(queryResult.size() == 2);
  queryResultIt = queryResult.begin();
  REQUIRE(queryResultIt->from().name() == "cd");
  REQUIRE(queryResultIt->to()->name() == "ab");
  queryResultIt++;
  REQUIRE(queryResultIt->from().name() == "ab");
  REQUIRE(queryResultIt->to()->name() == "cd");
}

TEST_CASE("Queries return correct result", "[basic]") {
  YAML::Node root = YAML::Load(
      "{city_001: {name: ab, x: 0, y: 0}, city_002: {name: abb, x: 0, y: 1}, "
      "city_003: {name: abbb, x: 0, y: 2}, city_004: {name: xabbb, x: 2, y:  "
      "2}, city_005: { name: aaabbb, x : 3, y : 3}}");
  const auto map = makeMap(root);

  SECTION("correcteness") {
    auto queryResult = query(map, {"ab"});
    auto queryResultIt = queryResult.begin();
    REQUIRE(queryResultIt->from().name() == "ab");
    REQUIRE(queryResultIt->to()->name() == "abb");

    queryResult = query(map, {"abb"});
    queryResultIt = queryResult.begin();
    REQUIRE(queryResultIt->from().name() == "abb");
    REQUIRE(queryResultIt->to()->name() == "ab");

    queryResult = query(map, {"abbb"});
    queryResultIt = queryResult.begin();
    REQUIRE(queryResultIt->from().name() == "abbb");
    REQUIRE(queryResultIt->to()->name() == "ab");

    queryResult = query(map, {"xabbb"});
    queryResultIt = queryResult.begin();
    REQUIRE(queryResultIt->from().name() == "xabbb");
    REQUIRE(queryResultIt->to()->name() == "abbb");
  }

  SECTION("result order") {
    auto queryResult = query(map, {"abbb", "xabbb"});
    auto queryResultIt = queryResult.begin();
    REQUIRE(queryResult.size() == 2);
    REQUIRE(queryResultIt->from().name() == "abbb");
    REQUIRE(queryResultIt->to()->name() == "ab");

    queryResultIt++;
    REQUIRE(queryResultIt->from().name() == "xabbb");
    REQUIRE(queryResultIt->to()->name() == "abbb");

    queryResult = query(map, {"xabbb", "abbb"});
    queryResultIt = queryResult.begin();
    REQUIRE(queryResult.size() == 2);
    REQUIRE(queryResultIt->from().name() == "xabbb");
    REQUIRE(queryResultIt->to()->name() == "abbb");

    queryResultIt++;
    REQUIRE(queryResultIt->from().name() == "abbb");
    REQUIRE(queryResultIt->to()->name() == "ab");
  }
}