#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace aoc {

std::vector<std::string> ReadLines(std::string filename) {
  auto file = std::ifstream(filename);
  auto result = std::vector<std::string>();

  auto line = std::string();
  while (std::getline(file, line)) {
    result.emplace_back(line);
  }

  return result;
}

}  // namespace aoc