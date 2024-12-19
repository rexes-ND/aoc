#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  std::string input_line;
  std::getline(input_file, input_line);
  for (char& c : input_line)
    if (c == ',') c = ' ';
  std::stringstream ss(input_line);
  std::string towel_pattern;
  std::set<std::string> towel_patterns;
  while (ss >> towel_pattern) towel_patterns.insert(towel_pattern);
  std::getline(input_file, input_line);

  std::vector<std::string> designs;
  while (input_file >> input_line) designs.push_back(input_line);

  int possible_designs = 0;
  long long different_designs = 0;

  for (auto design : designs) {
    std::vector<bool> dp(design.size() + 1, false);
    dp[design.size()] = true;
    for (int i = design.size() - 1; i >= 0; --i) {
      for (int j = i; j < design.size(); ++j) {
        if (dp[j + 1] && towel_patterns.count(design.substr(i, j - i + 1)))
          dp[i] = true;
      }
    }
    if (dp[0]) possible_designs++;
    std::vector<long long> diff_dp(design.size() + 1, 0);
    diff_dp[design.size()] = 1;
    for (int i = design.size() - 1; i >= 0; --i) {
      for (int j = i; j < design.size(); ++j) {
        if (towel_patterns.count(design.substr(i, j - i + 1)))
          diff_dp[i] += diff_dp[j + 1];
      }
    }
    different_designs += diff_dp[0];
  }

  std::cout << possible_designs << std::endl;
  std::cout << different_designs << std::endl;

  return 0;
}
