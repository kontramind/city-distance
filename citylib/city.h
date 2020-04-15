#pragma once

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <vector>

struct Location {
  int x{std::numeric_limits<int>::max()};
  int y{std::numeric_limits<int>::max()};
};

struct City {
  std::string name{"NONE"};
  Location coordinates;
};

class CityToCity {
public:
  CityToCity() = default;
  CityToCity(const City &from, const City &to) : fromCity(from), toCity(to){};
  City fromCity{};
  City toCity{};
};

using CityCollection = std::vector<City>;
using CityToCityCollection = std::vector<CityToCity>;
using CloseCitiesMap = std::map<std::string, CityToCity>;

std::ostream &operator<<(std::ostream &out, const City &city);
std::ostream &operator<<(std::ostream &out, const Location &coords);
std::ostream &operator<<(std::ostream &out, const CityCollection &cities);
std::ostream &operator<<(std::ostream &out, const CloseCitiesMap &cities);
std::ostream &operator<<(std::ostream &out, const CityToCityCollection &cities);

bool close(const City &lhs, const City &rhs);
int distance(const City &lhs, const City &rhs);
CloseCitiesMap makeMap(const YAML::Node &root);
CityToCityCollection query(const CloseCitiesMap &closeCityMap,
                           std::vector<std::string> queryCities);
