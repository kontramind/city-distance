#include "city.h"

#include <algorithm>
#include <stdexcept>

namespace Cities {

Location::Location(int x, int y) : m_x(x), m_y(y) {}
int Location::x() const { return m_x; }
int Location::y() const { return m_y; }

City::City(const std::string &name, const Location &location)
    : m_name(name), m_location(location) {}
std::string City::name() const { return m_name; }
Location City::location() const { return m_location; }

CityPair::CityPair(const City &from)
    : m_fromCity(from), m_toCity(std::nullopt){};
CityPair::CityPair(const City &from, const City &to)
    : m_fromCity(from), m_toCity(to){};
City CityPair::from() const { return m_fromCity; }
std::optional<City> CityPair::to() const { return m_toCity; }

std::ostream &operator<<(std::ostream &out, const Location &location) {
  out << "x: " << location.x() << ", y: " << location.y();
  return out;
}

std::ostream &operator<<(std::ostream &out, const City &city) {
  out << "name: " << city.name() << ", " << city.location();
  return out;
}

std::ostream &operator<<(std::ostream &out, const CityCollection &cities) {
  std::for_each(cities.cbegin(), cities.cend(),
                [&out](const auto &city) { out << city << '\n'; });
  return out;
}

std::ostream &operator<<(std::ostream &out, const CloseCitiesMap &cities) {
  std::for_each(cities.cbegin(), cities.cend(), [&out](const auto &cityPair) {
    out << cityPair.second.from().name() << " -> ";
    if (cityPair.second.to()) {
      out << cityPair.second.to()->name();
    } else {
      out << "NONE";
    }
    out << '\n';
  });
  return out;
}

std::ostream &operator<<(std::ostream &out, const CityPairCollection &cities) {
  std::for_each(cities.cbegin(), cities.cend(), [&out](const auto &cityPair) {
    out << cityPair.from().name() << " -> ";
    if (cityPair.to()) {
      out << cityPair.to()->name();
    } else {
      out << "NONE";
    }
    out << '\n';
  });
  return out;
}

bool close(const City &lhs, const City &rhs) {
  return lhs.location().x() == rhs.location().x() ||
         lhs.location().y() == rhs.location().y();
}

int distance(const City &lhs, const City &rhs) {
  return std::abs(lhs.location().x() - rhs.location().x()) +
         std::abs(lhs.location().y() - rhs.location().y());
}

CloseCitiesMap makeMap(const YAML::Node &root) {
  CityCollection cities;
  CloseCitiesMap closeCities;

  for (const auto &node : root) {
    const City newCity{
        {node.second["name"].as<std::string>()},
        {node.second["x"].as<int>(), node.second["y"].as<int>()}};
    cities.emplace_back(newCity);
    closeCities.emplace(newCity.name(), CityPair{newCity});

    for (const auto existingCity : cities) {
      if (close(existingCity, newCity) &&
          newCity.name() != existingCity.name()) {
        auto newIt = closeCities.find(newCity.name());
        auto existingIt = closeCities.find(existingCity.name());
        if (!existingIt->second.to()) {
          existingIt->second = CityPair(existingCity, newCity);
        } else {
          const auto newDistance = distance(existingCity, newCity);
          const auto existingDistance = distance(
              existingIt->second.from(), existingIt->second.to().value());
          if (newDistance < existingDistance) {
            existingIt->second = CityPair{existingCity, newCity};
          } else if (newDistance == existingDistance &&
                     newCity.name() != existingIt->second.to()->name() &&
                     newCity.name() < existingIt->second.to()->name()) {
            existingIt->second = CityPair{existingCity, newCity};
          }
        }
        if (!newIt->second.to()) {
          newIt->second = CityPair(newCity, existingCity);
        }
      }
    }
  }
  return closeCities;
}

CityPairCollection query(const CloseCitiesMap &closeCityMap,
                         std::vector<std::string> queryCities) {
  if (closeCityMap.empty()) {
    throw std::runtime_error("Query on empty map not possible");
  }

  CityPairCollection result;
  for (const auto &qCity : queryCities) {
    const auto it = closeCityMap.find(qCity);
    if (it != closeCityMap.end()) {
      result.push_back(it->second);
    } else {
      throw std::runtime_error("City not found");
    }
  }
  return result;
}
} // namespace Cities
