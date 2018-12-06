#include <set>
#include "Utils.hpp"

auto StringToIntegerVector(const std::vector<std::string> &strings) {
  auto result = std::vector<int>();

  for (auto &s : strings) {
    result.emplace_back(std::stoi(s));
  }

  return result;
}

int SolutionPart1(std::vector<int> &numbers) {
  auto result = 0;
  for (auto &x : numbers) {
    result += x;
  }
  return result;
}

int SolutionPart2(std::vector<int> &numbers) {
  auto previouslyFound = std::set<int>{0};
  auto result = 0;

  for (;;) {
    for (auto &x : numbers) {
      result += x;
      if (previouslyFound.find(result) != previouslyFound.end()) {
        return result;
      }
      previouslyFound.insert(result);
    }
  }
}

int main() {
  auto lines = aoc::ReadLines("Day1.txt");
  auto numbers = StringToIntegerVector(lines);

  auto solution1 = SolutionPart1(numbers);
  std::cout << "Day 1 Part 1 Solution:" << solution1 << std::endl;

  auto solution2 = SolutionPart2(numbers);
  std::cout << "Day 1 Part 2 Solution:" << solution2 << std::endl;

  return 0;
}