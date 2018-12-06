#include <regex>
#include <set>
#include "Utils.hpp"

struct Claim {
  int id;
  int X;
  int Y;
  int width;
  int height;
};

inline Claim MakeClaim(std::smatch& match) {
  return Claim{std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3]),
               std::stoi(match[4]), std::stoi(match[5])};
}

std::vector<Claim> ParseLines(const std::vector<std::string>& lines) {
  auto parse_line = std::regex("#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)");
  auto claims = std::vector<Claim>();

  for (auto& line : lines) {
    std::smatch match;

    std::regex_match(line, match, parse_line);
    claims.emplace_back(MakeClaim(match));
  }

  return claims;
}

std::pair<int, int> GetAnswers(const std::vector<Claim>& claims) {
  auto owner = std::vector<int>(1'000'000, -1);
  auto marked_in_count = std::vector<bool>(1'000'000);
  auto non_overlapping = std::set<int>();

  auto conflicting_claims = 0;

  auto flatten = [](int x, int y) { return y * 1000 + x; };

  for (auto& [id, X, Y, width, height] : claims) {
    auto overlapping = false;

    for (auto x = X; x < X + width; ++x) {
      for (auto y = Y; y < Y + height; ++y) {
        auto cell = flatten(x, y);
        if (owner[cell] == -1) {
          owner[cell] = id;
        } else if (!marked_in_count[cell]) {
          ++conflicting_claims;
          marked_in_count[cell] = true;
        }

        if (owner[cell] != id) {
          overlapping = true;
          non_overlapping.erase(owner[cell]);
        }
      }
    }

    if (!overlapping) {
      non_overlapping.insert(id);
    }
  }

  return {conflicting_claims, *non_overlapping.begin()};
}

int main() {
  auto lines = aoc::ReadLines("Day3.txt");
  auto claims = ParseLines(lines);

  auto [answer1, answer2] = GetAnswers(claims);

  std::cout << answer1 << std::endl;
  std::cout << answer2 << std::endl;

  return 0;
}