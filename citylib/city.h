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
  int insertionIndex{0};
};

class CityToCity {
public:
  CityToCity() = default;
  CityToCity(const City &from, const City &to) : fromCity(from), toCity(to){};
  City fromCity{};
  City toCity{};
};

struct CityToCityCompare {
  bool operator()(const CityToCity &lhs, const CityToCity &rhs) const {
    return lhs.fromCity.insertionIndex < rhs.fromCity.insertionIndex;
  }
};

using CityCollection = std::vector<City>;
using CloseCitiesMap = std::map<std::string, CityToCity>;
using SortedCloseCitiesMap = std::set<CityToCity, CityToCityCompare>;

std::ostream &operator<<(std::ostream &out, const City &city);
std::ostream &operator<<(std::ostream &out, const Location &coords);
std::ostream &operator<<(std::ostream &out, const CityCollection &cities);
std::ostream &operator<<(std::ostream &out, const CloseCitiesMap &cities);
std::ostream &operator<<(std::ostream &out, const SortedCloseCitiesMap &cities);

bool close(const City &lhs, const City &rhs);
int distance(const City &lhs, const City &rhs);
CloseCitiesMap makeMap(const YAML::Node &root);
SortedCloseCitiesMap query(const CloseCitiesMap &closeCityMap,
                           std::vector<std::string> queryCities);
