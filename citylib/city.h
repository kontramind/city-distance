#pragma once

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <limits>
#include <optional>
#include <set>
#include <unordered_map>
#include <vector>

namespace Cities {

class Location {
public:
  Location(int x, int y);
  int x() const;
  int y() const;

private:
  int m_x;
  int m_y;
};

class City {
public:
  City(const std::string &name, const Location &location);
  std::string name() const;
  Location location() const;

private:
  std::string m_name;
  Location m_location;
};

class CityPair {
public:
  explicit CityPair(const City &from);
  explicit CityPair(const City &from, const City &to);
  City from() const;
  std::optional<City> to() const;

private:
  City m_fromCity;
  std::optional<City> m_toCity{std::nullopt};
};

using CityCollection = std::vector<City>;
using CityPairCollection = std::vector<CityPair>;
using CloseCitiesMap = std::unordered_map<std::string, CityPair>;

std::ostream &operator<<(std::ostream &out, const City &city);
std::ostream &operator<<(std::ostream &out, const Location &location);
std::ostream &operator<<(std::ostream &out, const CityCollection &cities);
std::ostream &operator<<(std::ostream &out, const CloseCitiesMap &cities);
std::ostream &operator<<(std::ostream &out, const CityPairCollection &cities);

bool close(const City &lhs, const City &rhs);
int distance(const City &lhs, const City &rhs);
CloseCitiesMap makeMap(const YAML::Node &root);
CityPairCollection query(const CloseCitiesMap &closeCityMap,
                         std::vector<std::string> queryCities);

} // namespace Cities