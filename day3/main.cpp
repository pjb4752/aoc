#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

int find_priority(char c) {
  if (c >= 'a' && c <= 'z') {
    return c - 96;
  } else if (c >= 'A' && c <= 'Z') {
    return c - 38;
  } else {
    return 0;
  }
}

int calculate_part1(const std::vector<std::string>& lines) {
  auto total = 0;

  for (const auto& line : lines) {
    auto midway = line.length() / 2;
    auto lookup = std::unordered_set<char>{line.begin() + midway, line.end()};
    auto seen = std::unordered_set<char>{};

    for (auto it = line.begin(); it != (line.begin() + midway); it++) {
      if (lookup.count(*it) > 0 && seen.count(*it) == 0) {
        total += find_priority(*it);
        seen.insert(*it);
      }
    }
  }
  return total;
}

int calculate_part2(const std::vector<std::string>& lines) {
  auto total = 0;
  auto counter = 0;

  for (auto i = 0; i < lines.size(); i += 3) {
    auto second = std::unordered_set<char>{lines[i+1].begin(), lines[i+1].end()};
    auto third = std::unordered_set<char>{lines[i+2].begin(), lines[i+2].end()};

    for (auto it = lines[i].begin(); it != lines[i].end(); it++) {
      if (second.count(*it) > 0 && third.count(*it) > 0) {
        total += find_priority(*it);
        break;
      }
    }
  }

  return total;
}

int main(int argc, char** argv) {
  auto in = std::fstream{"input", std::ios::in};
  auto lines = std::vector<std::string>{};
  for (std::string line; getline(in, line);) {
    lines.push_back(line);
  }

  std::cout << "part 1: " << calculate_part1(lines) << std::endl;
  std::cout << "part 2: " << calculate_part2(lines) << std::endl;

  return 0;
}
