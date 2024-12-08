#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  std::string input_line;
  std::vector<std::string> antenna_map;
  while (std::getline(input_file, input_line))
    antenna_map.push_back(input_line);

  std::map<char, std::vector<std::pair<int, int>>> ch_to_ijs;
  for (int i = 0; i < antenna_map.size(); ++i) {
    for (int j = 0; j < antenna_map[i].size(); ++j) {
      char c = antenna_map[i][j];
      if (c == '.') continue;
      ch_to_ijs[c].push_back({i, j});
    }
  }
  std::vector<std::vector<bool>> antinode_exist;
  std::vector<std::vector<bool>> resonant_antinode_exist;
  for (int i = 0; i < antenna_map.size(); ++i) {
    antinode_exist.push_back(std::vector<bool>(antenna_map[i].size(), false));
    resonant_antinode_exist.push_back(
        std::vector<bool>(antenna_map[i].size(), false));
  }
  for (const auto& [ch, ijs] : ch_to_ijs) {
    for (int k = 0; k < ijs.size(); ++k) {
      for (int t = k + 1; t < ijs.size(); ++t) {
        auto [i1, j1] = ijs[k];
        auto [i2, j2] = ijs[t];
        int di = i2 - i1;
        int dj = j2 - j1;
        if (0 <= i1 - di && i1 - di < antenna_map.size() && 0 <= j1 - dj &&
            j1 - dj < antenna_map[i1 - di].size())
          antinode_exist[i1 - di][j1 - dj] = true;

        if (0 <= i2 + di && i2 + di < antenna_map.size() && 0 <= j2 + dj &&
            j2 + dj < antenna_map[i2 + di].size())
          antinode_exist[i2 + di][j2 + dj] = true;
        while (0 <= i1 && i1 < antenna_map.size() && 0 <= j1 &&
               j1 < antenna_map[i1].size()) {
          resonant_antinode_exist[i1][j1] = true;
          i1 -= di;
          j1 -= dj;
        }
        while (0 <= i2 && i2 < antenna_map.size() && 0 <= j2 &&
               j2 < antenna_map[i2].size()) {
          resonant_antinode_exist[i2][j2] = true;
          i2 += di;
          j2 += dj;
        }
      }
    }
  }
  int antinode_locations = 0;
  int resonant_antinode_locations = 0;
  for (int i = 0; i < antinode_exist.size(); ++i) {
    for (int j = 0; j < antinode_exist[i].size(); ++j) {
      if (antinode_exist[i][j]) antinode_locations++;
      if (resonant_antinode_exist[i][j]) resonant_antinode_locations++;
    }
  }
  std::cout << antinode_locations << std::endl;
  std::cout << resonant_antinode_locations << std::endl;

  return 0;
}
