#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

struct ClawMachine {
  std::pair<long long, long long> a;
  std::pair<long long, long long> b;
  std::pair<long long, long long> prize;

  long long get_min_tokens() {
    long long min_tokens = 0;
    auto [ax, ay] = a;
    auto [bx, by] = b;
    auto [px, py] = prize;
    long long aa = 0, bb = 0;
    if (ay * bx == ax * by) {
      if (ay * px != ax * py) return 0;
      auto g = std::gcd(ax, bx);
      if (px % g) return 0;
      ax /= g;
      bx /= g;
      px /= g;
      while ((px - ax * aa) % bx) ++aa;
      bb = (px - ax * aa) / bx;
    } else {
      if ((ay * px - ax * py) % (ay * bx - ax * by)) return 0;
      bb = (ay * px - ax * py) / (ay * bx - ax * by);
      if ((px - bx * bb) % ax) return 0;
      aa = (px - bx * bb) / ax;
      if (aa < 0 || bb < 0) return 0;
    }

    auto tokens = 3 * aa + bb;
    if (min_tokens == 0)
      min_tokens = tokens;
    else
      min_tokens = std::min(min_tokens, tokens);

    return min_tokens;
  }
};

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  std::regex button_pattern(R"(X\+(\d+), Y\+(\d+))");
  std::regex prize_pattern(R"(X\=(\d+), Y\=(\d+))");

  std::vector<ClawMachine> claw_machines;
  std::string a_line, b_line, prize_line, empty_line;
  while (std::getline(input_file, a_line)) {
    std::getline(input_file, b_line);
    std::getline(input_file, prize_line);
    std::getline(input_file, empty_line);

    std::smatch match;

    ClawMachine claw_machine;

    std::regex_search(a_line, match, button_pattern);
    claw_machine.a = {stoi(match[1]), stoi(match[2])};

    std::regex_search(b_line, match, button_pattern);
    claw_machine.b = {stoi(match[1]), stoi(match[2])};

    std::regex_search(prize_line, match, prize_pattern);
    claw_machine.prize = {stoi(match[1]), stoi(match[2])};

    claw_machines.push_back(claw_machine);
  }

  long long tokens = 0;
  for (auto claw_machine : claw_machines)
    tokens += claw_machine.get_min_tokens();
  std::cout << tokens << std::endl;

  tokens = 0;
  for (auto claw_machine : claw_machines) {
    claw_machine.prize.first += 10000000000000LL;
    claw_machine.prize.second += 10000000000000LL;
    tokens += claw_machine.get_min_tokens();
  }
  std::cout << tokens << std::endl;

  return 0;
}
