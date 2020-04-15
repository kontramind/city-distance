#include <city.h>
#include <yaml-cpp/yaml.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) {
  const auto closeCities = Cities::makeMap(YAML::LoadFile("data/test_000.yml"));
  const auto result = Cities::query(closeCities, {"ab", "aaabbb"});
  std::cout << result << '\n';

  return 0;
}
