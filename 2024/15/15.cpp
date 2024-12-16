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
  std::vector<std::string> warehouse;
  std::vector<std::string> scaled_warehouse;
  while (std::getline(input_file, input_line)) {
    if (input_line.empty()) break;
    warehouse.push_back(input_line);
    std::string scaled_line;
    for (char c : input_line) {
      if (c == '#')
        scaled_line += "##";
      else if (c == 'O')
        scaled_line += "[]";
      else if (c == '.')
        scaled_line += "..";
      else if (c == '@')
        scaled_line += "@.";
    }
    scaled_warehouse.push_back(scaled_line);
  }
  std::string movements;
  while (std::getline(input_file, input_line)) movements += input_line;

  int ri, rj;
  for (int i = 0; i < warehouse.size(); ++i) {
    for (int j = 0; j < warehouse[i].size(); ++j) {
      if (warehouse[i][j] == '@') {
        ri = i;
        rj = j;
      }
    }
  }
  std::map<char, std::pair<int, int>> mvdirs = {
      {'^', {-1, 0}}, {'>', {0, 1}}, {'v', {1, 0}}, {'<', {0, -1}}};
  for (auto mv : movements) {
    auto [di, dj] = mvdirs[mv];
    auto i = ri + di;
    auto j = rj + dj;
    while (warehouse[i][j] == 'O') {
      i += di;
      j += dj;
    }
    if (warehouse[i][j] == '#') continue;
    while (true) {
      if (i == ri && j == rj) break;
      std::swap(warehouse[i][j], warehouse[i - di][j - dj]);
      i -= di;
      j -= dj;
    }
    ri = i + di;
    rj = j + dj;
  }

  long long gps_coordinates_sum = 0;
  for (int i = 0; i < warehouse.size(); ++i) {
    for (int j = 0; j < warehouse[i].size(); ++j) {
      if (warehouse[i][j] == 'O') gps_coordinates_sum += 100 * i + j;
    }
  }
  std::cout << gps_coordinates_sum << std::endl;

  for (int i = 0; i < scaled_warehouse.size(); ++i) {
    for (int j = 0; j < scaled_warehouse[i].size(); ++j) {
      if (scaled_warehouse[i][j] == '@') {
        ri = i;
        rj = j;
      }
    }
  }

  for (auto mv : movements) {
    auto [di, dj] = mvdirs[mv];
    auto mv_pos = true;
    std::map<std::pair<int, int>, char> visit;
    std::vector<std::pair<int, int>> cur_level = {{ri, rj}};
    visit[{ri, rj}] = '@';
    while (cur_level.size() && mv_pos) {
      std::vector<std::pair<int, int>> next_level;
      for (auto [bi, bj] : cur_level) {
        bi += di;
        bj += dj;
        if (scaled_warehouse[bi][bj] == '#')
          mv_pos = false;
        else if (scaled_warehouse[bi][bj] == '[') {
          if (!visit.count({bi, bj})) next_level.push_back({bi, bj});
          if (!visit.count({bi, bj + 1})) next_level.push_back({bi, bj + 1});
          visit[{bi, bj}] = '[';
          visit[{bi, bj + 1}] = ']';
        } else if (scaled_warehouse[bi][bj] == ']') {
          if (!visit.count({bi, bj})) next_level.push_back({bi, bj});
          if (!visit.count({bi, bj - 1})) next_level.push_back({bi, bj - 1});
          visit[{bi, bj}] = ']';
          visit[{bi, bj - 1}] = '[';
        }
      }
      cur_level = next_level;
    }
    if (!mv_pos) continue;
    for (auto [ij, c] : visit) {
      auto [i, j] = ij;
      scaled_warehouse[i][j] = '.';
    }
    for (auto [ij, c] : visit) {
      auto [i, j] = ij;
      scaled_warehouse[i + di][j + dj] = c;
    }
    ri += di;
    rj += dj;
  }

  long long final_gps_coordinates_sum = 0;
  for (int i = 0; i < scaled_warehouse.size(); ++i) {
    for (int j = 0; j < scaled_warehouse[i].size(); ++j) {
      if (scaled_warehouse[i][j] == '[')
        final_gps_coordinates_sum += 100 * i + j;
    }
  }
  std::cout << final_gps_coordinates_sum << std::endl;

  return 0;
}
