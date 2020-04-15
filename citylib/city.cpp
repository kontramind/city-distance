#include "city.h"

#include <algorithm>
#include <stdexcept>

namespace Cities {
std::ostream &operator<<(std::ostream &out, const Location &coords) {
  out << "x: " << coords.x << ", y: " << coords.y;
  return out;
}

std::ostream &operator<<(std::ostream &out, const City &city) {
  out << "name: " << city.name << ", " << city.coordinates;
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

std::ostream &operator<<(std::ostream &out, const CityPairCollection &cities) {
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

  for (const auto &node : root) {
    const City newCity{
        {node.second["name"].as<std::string>()},
        {node.second["x"].as<int>(), node.second["y"].as<int>()}};

    cities.emplace_back(newCity);
    for (const auto existingCity : cities) {
      if (existingCity.name == newCity.name) {
        continue;
      }

      if (close(existingCity, newCity)) {
        const auto it = closeCities.find(existingCity.name);
        if (it == closeCities.end()) {
          closeCities[existingCity.name] = CityPair{existingCity, newCity};
          closeCities[newCity.name] = CityPair{newCity, existingCity};
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

CityPairCollection query(const CloseCitiesMap &closeCityMap,
                         std::vector<std::string> queryCities) {
  if (closeCityMap.empty()) {
    throw std::runtime_error("Query on emmpty map not possible");
  }

  CityPairCollection result;
  for (const auto &qCity : queryCities) {
    const auto it = closeCityMap.find(qCity);
    if (it != closeCityMap.end()) {
      result.emplace_back(it->second);
    }
  }
  return result;
}
} // namespace Cities
