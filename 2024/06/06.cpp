#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

const std::vector<char> dir_chs = {'^', '>', 'v', '<'};
std::map<char, std::pair<int, int>> dirs = {{'^', std::make_pair(-1, 0)},
                                            {'>', std::make_pair(0, 1)},
                                            {'v', std::make_pair(1, 0)},
                                            {'<', std::make_pair(0, -1)}};

char get_next_dir_ch(char cur_dir_ch) {
  int i = 0;
  while (dir_chs[i] != cur_dir_ch) ++i;
  i = (i + 1) % 4;
  return dir_chs[i];
}

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  std::string input_line;
  std::vector<std::string> guard_map;
  while (std::getline(input_file, input_line)) guard_map.push_back(input_line);

  int guard_i = 0, guard_j = 0;
  int init_guard_i = 0, init_guard_j = 0;
  char guard_ch = '^', init_guard_ch = '^';
  for (int i = 0; i < guard_map.size(); ++i) {
    for (int j = 0; j < guard_map[i].size(); ++j) {
      char ch = guard_map[i][j];
      if (ch != '.' && ch != '#') {
        init_guard_i = i;
        init_guard_j = j;
        init_guard_ch = ch;
        guard_i = i;
        guard_j = j;
        guard_ch = ch;
        break;
      }
    }
  }

  int distinct_positions = 0;
  std::vector<std::vector<bool>> visit;
  for (int i = 0; i < guard_map.size(); ++i)
    visit.push_back(std::vector<bool>(guard_map[i].size(), false));

  while (0 <= guard_i && guard_i < guard_map.size() && 0 <= guard_j &&
         guard_j < guard_map[guard_i].size()) {
    if (!visit[guard_i][guard_j]) {
      visit[guard_i][guard_j] = true;
      ++distinct_positions;
    }

    auto [di, dj] = dirs[guard_ch];
    if (0 <= guard_i + di && guard_i + di < guard_map.size() &&
        0 <= guard_j + dj && guard_j + dj < guard_map[guard_i + di].size() &&
        guard_map[guard_i + di][guard_j + dj] == '#') {
      guard_ch = get_next_dir_ch(guard_ch);
      std::tie(di, dj) = dirs[guard_ch];
    } else {
      guard_i += di;
      guard_j += dj;
    }
  }
  std::cout << distinct_positions << std::endl;

  int obstructing_positions = 0;
  for (int oi = 0; oi < guard_map.size(); ++oi) {
    for (int oj = 0; oj < guard_map[oi].size(); ++oj) {
      if (guard_map[oi][oj] != '.') continue;
      guard_map[oi][oj] = '#';

      std::map<char, std::vector<std::vector<bool>>> dir_visit;
      for (auto dir_ch : dir_chs) {
        for (int i = 0; i < guard_map.size(); ++i)
          dir_visit[dir_ch].push_back(
              std::vector<bool>(guard_map[i].size(), false));
      }
      guard_i = init_guard_i;
      guard_j = init_guard_j;
      guard_ch = init_guard_ch;
      bool is_obstructing = false;

      while (0 <= guard_i && guard_i < guard_map.size() && 0 <= guard_j &&
             guard_j < guard_map[guard_i].size()) {
        if (dir_visit[guard_ch][guard_i][guard_j]) {
          is_obstructing = true;
          break;
        }
        dir_visit[guard_ch][guard_i][guard_j] = true;

        auto [di, dj] = dirs[guard_ch];
        if (0 <= guard_i + di && guard_i + di < guard_map.size() &&
            0 <= guard_j + dj &&
            guard_j + dj < guard_map[guard_i + di].size() &&
            guard_map[guard_i + di][guard_j + dj] == '#') {
          guard_ch = get_next_dir_ch(guard_ch);
          std::tie(di, dj) = dirs[guard_ch];
        } else {
          guard_i += di;
          guard_j += dj;
        }
      }

      if (is_obstructing) ++obstructing_positions;

      guard_map[oi][oj] = '.';
    }
  }
  std::cout << obstructing_positions << std::endl;

  return 0;
}
