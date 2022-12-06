#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

struct Range {
  Range(int lower_bound, int upper_bound) :
    lower_bound_(lower_bound),
    upper_bound_(upper_bound) {}

  bool contains(const Range& other) const {
    return (lower_bound_ <= other.lower_bound_) && (upper_bound_ >= other.upper_bound_);
  }

  bool overlaps(const Range& other) const {
    return (upper_bound_ >= other.lower_bound_) && (lower_bound_ <= other.upper_bound_);
  }

  private:
    int lower_bound_, upper_bound_;
};

std::string::size_type find_or_error(const std::string& str, char delim, std::string::size_type start = 0) {
  auto position = str.find(delim, start);
  if (position == std::string::npos) {
    throw std::runtime_error{"bad input"};
  }

  return position;
}

Range parse_range(const std::string& line, std::string::size_type start, std::string::size_type end) {
  auto delimiter_position = find_or_error(line, '-', start);

  auto lower_bound = std::stoi(line.substr(start, delimiter_position));
  auto upper_bound = std::stoi(line.substr(delimiter_position + 1, end));

  return Range{lower_bound, upper_bound};
}

int calculate_part1(const std::vector<std::pair<Range, Range>>& range_pairs) {
  return std::count_if(range_pairs.begin(), range_pairs.end(), [](const auto& range_pair) {
      const auto& [left_range, right_range] = range_pair;
      return left_range.contains(right_range) || right_range.contains(left_range);
    });
}

int calculate_part2(const std::vector<std::pair<Range, Range>>& range_pairs) {
  return std::count_if(range_pairs.begin(), range_pairs.end(), [](const auto& range_pair) {
      const auto& [left_range, right_range] = range_pair;
      return left_range.overlaps(right_range) || right_range.overlaps(left_range);
    });
}

int main(int argc, char** argv) {
  auto in = std::fstream{"input", std::ios::in};
  auto range_pairs = std::vector<std::pair<Range, Range>>{};

  for (std::string line; getline(in, line);) {
    auto delimeter_position = find_or_error(line, ',');
    auto first_range = parse_range(line, 0, delimeter_position);
    auto last_range = parse_range(line, delimeter_position + 1, line.length());

    range_pairs.emplace_back(first_range, last_range);
  }

  std::cout << "part 1: " << calculate_part1(range_pairs) << std::endl;
  std::cout << "part 2: " << calculate_part2(range_pairs) << std::endl;

  return 0;
}
