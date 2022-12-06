#include <cassert>
#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <utility>
#include <vector>

struct Move {
  Move(int start, int end, int amount) :
    start(start),
    end(end),
    amount(amount) {}

  int start, end, amount;
};

unsigned int count_labels(const std::string& line) {
  auto in = std::istringstream{line};
  auto label = 0;
  unsigned int i = 0;

  while (in >> label) {
    i++;
  }

  return i;
}

void parse_setup(std::vector<std::string>::const_iterator begin,
                 std::vector<std::string>::const_iterator end,
                 std::vector<std::deque<char>>& destination) {

  for (auto it = begin; it != end; it++) {
    auto i = 0;
    while (true) {
      auto possible_char = it->at(i * 4 + 1);
      if (possible_char >= 'A' && possible_char <= 'Z') {
        destination[i].push_front(possible_char);
      }

      if (++i * 4 > it->length()) {
        break;
      }
    }
  }
}

void parse_moves(std::vector<std::string>::const_iterator begin,
                 std::vector<std::string>::const_iterator end,
                 std::vector<Move>& destination) {

  for (auto it = begin; it != end; it++) {
    int amount, start, end;

    std::sscanf(it->c_str(), "move %d from %d to %d", &amount, &start, &end);
    destination.emplace_back(start, end, amount);
  }
}

void move_single_crates(std::deque<char>& source, std::deque<char>& destination, int amount) {
  for (auto i = 0; i < amount; i++) {
    assert(!source.empty());

    destination.push_back(source.back());
    source.pop_back();
  }
}

void move_crate_stack(std::deque<char>& source, std::deque<char>& destination, int amount) {
  auto tmp = std::stack<char>{};

  for (auto i = 0; i < amount; i++) {
    assert(!source.empty());

    tmp.push(source.back());
    source.pop_back();
  }

  while (!tmp.empty()) {
    destination.push_back(tmp.top());
    tmp.pop();
  }
}

template <typename F>
void find_crate_positions(const std::vector<std::deque<char>> stacks,
                          const std::vector<Move>& moves,
                          F&& crate_mover) {
  auto stack_copy = stacks;
  for (const auto& m: moves) {
    crate_mover(stack_copy[m.start - 1], stack_copy[m.end - 1], m.amount);
  }

  auto out = std::ostringstream{};
  for (const auto& s: stack_copy) {
    out << s.back();
  }

  std::cout << out.str() << std::endl;
}

int main(int argc, char** argv) {
  auto in = std::fstream{"input", std::ios::in};
  auto lines = std::vector<std::string>{};

  auto counter = 0;
  auto divider = 0;
  for (std::string line; getline(in, line);) {
    if (line.empty()) {
      divider = counter;
    } else {
      lines.push_back(line);
    }

    counter++;
  }

  auto setup_end = lines.begin() + (divider - 1);
  auto num_labels = count_labels(*setup_end);
  auto stacks = std::vector<std::deque<char>>{num_labels};
  parse_setup(lines.begin(), setup_end, stacks);

  auto moves = std::vector<Move>{};
  parse_moves(lines.begin() + divider, lines.end(), moves);

  find_crate_positions(stacks, moves, move_single_crates);
  find_crate_positions(stacks, moves, move_crate_stack);

  return 0;
}
