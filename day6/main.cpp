#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

int find_marker(std::istream&& in, int count) {
  auto values = std::deque<char>{};
  auto tmp = std::unordered_set<char>{};
  auto value = '\0';
  auto counter = 0;

  while (in >> value) {
    values.push_back(value);
    counter++;

    if (values.size() > count) {
      values.pop_front();
    }

    tmp.clear();
    tmp.insert(values.begin(), values.end());

    if (tmp.size() > (count - 1)) {
      return counter;
    }
  }

  return -1;
}

int main(int argc, char** argv) {
  auto in = std::fstream{"input", std::ios::in};
  auto input = std::string{};

  if (!getline(in, input)) {
    return 1;
  }

  std::cout << "part 1: " << find_marker(std::istringstream{input}, 4) << std::endl;
  std::cout << "part 2: " << find_marker(std::istringstream{input}, 14) << std::endl;

  return 0;
}
