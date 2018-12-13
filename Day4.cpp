#include <regex>
#include <numeric>
#include <optional>
#include "Utils.hpp"

enum LogType { GuardStarting, Asleep, WakingUp };

struct Timestamp {
  int month, day, hour, minute;
};

bool operator<(const Timestamp& lhs, const Timestamp& rhs) {
  return std::tie(lhs.month, lhs.day, lhs.hour, lhs.minute) <
         std::tie(rhs.month, rhs.day, rhs.hour, rhs.minute);
}

struct LogEntry {
  int id;
  LogType type;

  Timestamp time;
};

bool operator<(const LogEntry& lhs, const LogEntry& rhs) {
  return std::tie(lhs.id, lhs.time) < std::tie(rhs.id, rhs.time);
}

std::vector<LogEntry> ParseInput(std::vector<std::string>& lines) {
  std::sort(lines.begin(), lines.end());

  auto result = std::vector<LogEntry>();

  auto re_parse_timestamp =
      std::regex("\\[(\\d+)-(\\d+)-(\\d+) (\\d+):(\\d+)\\]");
  auto re_guard_start = std::regex("Guard #(\\d+) begins shift");

  auto guardId = -1;

  for (auto& line : lines) {
    std::smatch match;
    std::regex_search(line, match, re_parse_timestamp);

    auto day = std::stoi(match[3]), month = std::stoi(match[2]);
    auto hour = std::stoi(match[4]), minute = std::stoi(match[5]);

    auto log_type = LogType::GuardStarting;
    if (std::regex_search(line, match, re_guard_start)) {
      guardId = std::stoi(match[1]);
    } else if (line.find("wakes up") != std::string::npos) {
      log_type = LogType::WakingUp;
    } else {
      log_type = LogType::Asleep;
    }

    result.emplace_back(LogEntry{guardId, log_type, Timestamp{month, day, hour, minute}});
  }

  return result;
}

std::pair<int, std::array<int, 60>> ProcessGuard(std::vector<LogEntry>::iterator& it, 
                                                 const std::vector<LogEntry>::iterator& end_it) {
  auto guard_id = it->id;
  std::optional<Timestamp> fell_asleep = std::nullopt;
  auto was_asleep = std::array<int, 60>();

  auto record_as_sleeping = [&](int from, int to) {
    for (auto minute = from; minute < to; ++minute) {
      ++was_asleep[minute];
    }
  };

  auto mark_as_asleep = [&](const Timestamp& it) {
    if (it.day != fell_asleep->day) {
      record_as_sleeping(fell_asleep->minute, 60);
      record_as_sleeping(0, it.minute);
    } else {
      record_as_sleeping(fell_asleep->minute, it.minute);
    }
  };

  while (it->id == guard_id && it != end_it) {
    switch(it->type) {
      case GuardStarting: break;
      case Asleep:
        fell_asleep = it->time;
        break;
      case WakingUp:
        mark_as_asleep(it->time);
        break;
    }
    ++it;
  }

  return {guard_id, was_asleep};
}

inline int IndexWithGreatestFrequency(std::array<int, 60>& v) {
  return std::distance(v.begin(), std::max_element(v.begin(), v.end()));
}

/*
  Hello there reader,

  Notice that the functions SolvePart1 and SolvePart2 don't follow the DRY principle.
  It would indeed be trivial to merge them into 1 funciton, or if C++ functional 
  style wasn't so fidgety / I was better it, this could look quite nice.

  Also the naming conventions for the best global and local maximums isn't great either.
  But alas if you did press me on this I could probably make it look nicer, but the point
  of AOC is to practice problem sovling not amazing production-level code :) 
*/

int SolvePart1(std::vector<LogEntry>& entries) {
  std::sort(entries.begin(), entries.end());

  auto winner_id = 0, total_mins_slept = -1, sleepiest_min = 0;

  for (auto it = entries.begin(); it != entries.end();) {
    auto [guard_id, minutes_slept] = ProcessGuard(it, entries.end());
    auto most_slept_min = IndexWithGreatestFrequency(minutes_slept); 
    auto total_minutes = std::accumulate(minutes_slept.begin(), minutes_slept.end(), 0);

    if (total_minutes > total_mins_slept) {
      winner_id = guard_id;
      total_mins_slept = total_minutes;
      sleepiest_min = most_slept_min;
    }
  }

  return winner_id * sleepiest_min;
}

int SolvePart2(std::vector<LogEntry>& entries) {
  std::sort(entries.begin(), entries.end());

  auto winner_id = 0, most_slept_minute = 0, most_slept_frequency = 0;

  for (auto it = entries.begin(); it != entries.end();) {
    auto [guard_id, minutes_slept] = ProcessGuard(it, entries.end());
    auto slept_min = IndexWithGreatestFrequency(minutes_slept);
    auto slept_min_freq = minutes_slept[slept_min];

    if (slept_min_freq > most_slept_frequency) {
      winner_id = guard_id;
      most_slept_frequency = slept_min_freq;
      most_slept_minute = slept_min;
    }
  }

  return winner_id * most_slept_minute;
}

int main() {
  auto lines = aoc::ReadLines("Day4.txt");
  auto entries = ParseInput(lines);

  auto answer1 = SolvePart1(entries);
  auto answer2 = SolvePart2(entries);

  std::cout << "Answer Part 1:" << answer1 << std::endl;
  std::cout << "Answer Part 2:" << answer2 << std::endl;
}
