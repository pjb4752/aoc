#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

std::vector<int> sum_totals(std::istream& in) {
  auto totals = std::vector<int>{};
  auto total = 0;

  for (std::string line; getline(in, line);) {
    if (line.empty()) {
      totals.push_back(total);
      total = 0;
      continue;
    }

    total += std::stoi(line);
  }
  return totals;
}

int find_top_n(const std::vector<int>& sorted, int count) {
  return std::accumulate(sorted.end() - count, sorted.end(), 0);
}

int main(int argc, char** argv) {
  auto in = std::fstream{"input", std::ios::in};
  auto totals = sum_totals(in);

  std::sort(totals.begin(), totals.end());

  std::cout << "part 1: " << find_top_n(totals, 1) << std::endl;
  std::cout << "part 2: " << find_top_n(totals, 3) << std::endl;

  return 0;
}
