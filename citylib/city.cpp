#include "city.h"

#include <algorithm>

std::ostream &operator<<(std::ostream &out, const Location &coords) {
  out << "x: " << coords.x << ", y: " << coords.y;
  return out;
}

std::ostream &operator<<(std::ostream &out, const City &city) {
  out << "[" << city.insertionIndex << "] name: " << city.name << ", "
      << city.coordinates;
  return out;
}

std::ostream &operator<<(std::ostream &out, const CityCollection &cities) {
  std::for_each(cities.cbegin(), cities.cend(),
                [&out](const auto &city) { out << city << '\n'; });
  return out;
}

std::ostream &operator<<(std::ostream &out, const CloseCitiesMap &cities) {
  std::for_each(cities.cbegin(), cities.cend(), [&out](const auto &cityPair) {
    out << cityPair.second.fromCity.name << " -> "
        << cityPair.second.toCity.name << '\n';
  });
  return out;
}

std::ostream &operator<<(std::ostream &out,
                         const SortedCloseCitiesMap &cities) {
  std::for_each(cities.cbegin(), cities.cend(), [&out](const auto &cityPair) {
    out << cityPair.fromCity.name << " -> " << cityPair.toCity.name << '\n';
  });
  return out;
}

bool close(const City &lhs, const City &rhs) {
  return lhs.coordinates.x == rhs.coordinates.x ||
         lhs.coordinates.y == rhs.coordinates.y;
}

int distance(const City &lhs, const City &rhs) {
  return std::abs(lhs.coordinates.x - rhs.coordinates.x) +
         std::abs(lhs.coordinates.y - rhs.coordinates.y);
}

CloseCitiesMap makeMap(const YAML::Node &root) {
  CityCollection cities;
  CloseCitiesMap closeCities;

  int insertionIndex{0};
  for (const auto &node : root) {
    const City newCity{{node.second["name"].as<std::string>()},
                       {node.second["x"].as<int>(), node.second["y"].as<int>()},
                       insertionIndex++};

    cities.emplace_back(newCity);
    for (const auto existingCity : cities) {
      if (existingCity.name == newCity.name) {
        continue;
      }

      if (close(existingCity, newCity)) {
        const auto it = closeCities.find(existingCity.name);
        if (it == closeCities.end()) {
          closeCities[existingCity.name] = CityToCity{existingCity, newCity};
          closeCities[newCity.name] = CityToCity{newCity, existingCity};
        } else {
          const auto newDistance = distance(existingCity, newCity);
          const auto existingDistance =
              distance(it->second.fromCity, it->second.toCity);
          if (newDistance < existingDistance) {
            closeCities[existingCity.name] = {existingCity, newCity};
          } else if (newDistance == existingDistance &&
                     newCity.name < it->second.toCity.name) {
            closeCities[existingCity.name] = {existingCity, newCity};
          }
        }
      } else {
        closeCities[newCity.name] = {newCity, City{}};
      }
    }
  }
  return closeCities;
}

SortedCloseCitiesMap query(const CloseCitiesMap &closeCityMap,
                           std::vector<std::string> queryCities) {
  SortedCloseCitiesMap sortedCloseCities;
  for (const auto &qCity : queryCities) {
    const auto it = closeCityMap.find(qCity);
    if (it != closeCityMap.end()) {
      sortedCloseCities.insert(it->second);
    }
  }
  return sortedCloseCities;
}