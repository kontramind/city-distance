#include <city.h>
#include <yaml-cpp/yaml.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) {
  const auto closeCities = makeMap(YAML::LoadFile("data/test_000.yml"));
  const auto sortedCloseCities = query(closeCities, {"ab", "aaabbb"});
  std::cout << sortedCloseCities << '\n';

  return 0;
}
