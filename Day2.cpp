#include <algorithm>
#include <iterator>
#include <unordered_map>
#include "Utils.hpp"

int AnswerPart1(const std::vector<std::string> &lines) {
  auto numberOf2s = 0, numberOf3s = 0;

  for (auto &line : lines) {
    auto frequencies = std::unordered_map<char, int>();
    for (auto letter : line) {
      frequencies[letter] = frequencies[letter] + 1;
    }

    auto has2Letters = std::any_of(frequencies.begin(), frequencies.end(),
                                   [](auto it) { return it.second == 2; });

    auto has3Letters = std::any_of(frequencies.begin(), frequencies.end(),
                                   [](auto it) { return it.second == 3; });

    if (has3Letters) {
      ++numberOf3s;
    }

    if (has2Letters) {
      ++numberOf2s;
    }
  }

  return numberOf2s * numberOf3s;
}

std::string GetDifferenceString(const std::string &id1,
                                const std::string &id2) {
  std::string s;

  for (auto i = 0; i < id1.size(); ++i) {
    if (id1[i] == id2[i]) {
      s += id1[i];
    }
  }

  return s;
}

std::string AnswerPart2(std::vector<std::string> &lines) {
  std::sort(lines.begin(), lines.end());

  for (auto i = 0; i < lines.size() - 1; ++i) {
    auto id1 = lines[i], id2 = lines[i + 1];

    auto uniqueStr = GetDifferenceString(id1, id2);

    if (uniqueStr.size() == id1.size() - 1) {
      return uniqueStr;
    }
  }

  return std::string();
}

int main() {
  auto lines = aoc::ReadLines("Day2.txt");

  auto answer1 = AnswerPart1(lines);
  auto answer2 = AnswerPart2(lines);

  std::cout << "Day 1:" << answer1 << std::endl;
  std::cout << "Day 2:" << answer2 << std::endl;
}