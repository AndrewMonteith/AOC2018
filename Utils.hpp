#ifndef H_UTILS
#define H_UTILS
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <map>

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

std::string ReadLine(std::string filename) {
	auto file = std::ifstream(filename);

	auto line = std::string();
	std::getline(file, line);
	return line;
}

}  // namespace aoc
#endif
