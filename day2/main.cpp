#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

template<typename T>
using InputMapping = std::unordered_map<char, T>;

template <typename T>
using ScoreMapping = std::unordered_map<T, int>;

enum class Move { rock, paper, scissors };
enum class Outcome { won, tied, lost };

Move losing_move(Move m) {
  if (m == Move::rock) {
    return Move::scissors;
  } else if (m == Move::paper) {
    return Move::rock;
  } else {
    return Move::paper;
  }
}

Move winning_move(Move m) {
  if (m == Move::rock) {
    return Move::paper;
  } else if (m == Move::paper) {
    return Move::scissors;
  } else {
    return Move::rock;
  }
}

Outcome determine_outcome(Move opponent, Move target) {
  if (target == opponent) {
    return Outcome::tied;
  } else if (losing_move(target) == opponent) {
    return Outcome::won;
  } else {
    return Outcome::lost;
  }
}

int score_mapped_turn(
    const ScoreMapping<Move>& move_scores,
    const ScoreMapping<Outcome>& outcome_scores,
    Move opponent_move,
    Move my_move) {

  auto outcome = determine_outcome(opponent_move, my_move);
  return move_scores.at(my_move) + outcome_scores.at(outcome);
}

Move determine_needed_move(Move opponent, Outcome desired_outcome) {
  if (desired_outcome == Outcome::tied) {
    return opponent;
  } else if (desired_outcome == Outcome::won) {
    return winning_move(opponent);
  } else {
    return losing_move(opponent);
  }
}

int score_computed_turn(
    const ScoreMapping<Move>& move_scores,
    const ScoreMapping<Outcome>& outcome_scores,
    Move opponent_move,
    Outcome desired_outcome) {

  auto needed_move = determine_needed_move(opponent_move, desired_outcome);
  return move_scores.at(needed_move) + outcome_scores.at(desired_outcome);
}

template <typename T, typename F>
int calculate(
    const ScoreMapping<Move>& move_scores,
    const ScoreMapping<Outcome>& outcome_scores,
    const InputMapping<Move>& opponent_mapping,
    const InputMapping<T>& my_mapping,
    const std::vector<std::pair<char, char>>& input,
    F&& calculator) {

  int score = 0;
  for (const auto& values: input) {
    auto opponent_move = opponent_mapping.at(values.first);
    auto t = my_mapping.at(values.second);

    score += calculator(move_scores, outcome_scores, opponent_move, t);
  }

  return score;
}

void calculate_part1(
    const ScoreMapping<Move>& move_scores,
    const ScoreMapping<Outcome>& outcome_scores,
    const InputMapping<Move>& opponent_mapping,
    const std::vector<std::pair<char, char>>& input) {

  auto my_mapping = InputMapping<Move>{{'X', Move::rock}, {'Y', Move::paper}, {'Z', Move::scissors}};
  auto score = calculate(move_scores, outcome_scores, opponent_mapping, my_mapping, input, score_mapped_turn);

  std::cout << "part 1 score is: " << score << std::endl;
}

void calculate_part2(
    const ScoreMapping<Move>& move_scores,
    const ScoreMapping<Outcome>& outcome_scores,
    const InputMapping<Move>& opponent_mapping,
    const std::vector<std::pair<char, char>>& input) {

  auto my_mapping = InputMapping<Outcome>{{'X', Outcome::lost}, {'Y', Outcome::tied}, {'Z', Outcome::won}};
  auto score = calculate(move_scores, outcome_scores, opponent_mapping, my_mapping, input, score_computed_turn);

  std::cout << "part 2 score is: " << score << std::endl;
}

int main(int argc, char** argv) {
  auto in = std::fstream{"input", std::ios::in};
  auto opponent_code = '\0', my_code = '\0';
  auto input = std::vector<std::pair<char, char>>{};
  while (in >> opponent_code >> my_code) {
    input.emplace_back(opponent_code, my_code);
  }

  auto move_scores = ScoreMapping<Move>{{Move::rock, 1}, {Move::paper, 2}, {Move::scissors, 3}};
  auto outcome_scores = ScoreMapping<Outcome>{{Outcome::won, 6}, {Outcome::tied, 3}, {Outcome::lost, 0}};
  auto opponent_mapping = InputMapping<Move>{{'A', Move::rock}, {'B', Move::paper}, {'C', Move::scissors}};

  calculate_part1(move_scores, outcome_scores, opponent_mapping, input);
  calculate_part2(move_scores, outcome_scores, opponent_mapping, input);

  return 0;
}
