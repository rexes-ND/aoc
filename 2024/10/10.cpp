#include <fstream>
#include <iostream>
#include <map>
#include <vector>

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  std::vector<std::vector<int>> topographic_map;
  std::string input_line;
  while (std::getline(input_file, input_line)) {
    std::vector<int> line;
    for (char c : input_line) line.push_back(c - '0');
    topographic_map.push_back(line);
  }

  std::vector<std::pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

  long long trailhead_scores_sum = 0;
  long long trailhead_ratings_sum = 0;
  for (int i = 0; i < topographic_map.size(); ++i) {
    for (int j = 0; j < topographic_map[i].size(); ++j) {
      if (topographic_map[i][j]) continue;
      int cur = 0;
      std::map<std::pair<int, int>, int> cur_level;
      cur_level[{i, j}] = 1;
      while (cur < 9 && cur_level.size()) {
        ++cur;
        std::map<std::pair<int, int>, int> next_level;
        for (auto [ij, rt] : cur_level) {
          auto [i, j] = ij;
          for (auto [di, dj] : dirs) {
            if (0 <= i + di && i + di < topographic_map.size() && 0 <= j + dj &&
                j + dj < topographic_map[i + di].size() &&
                topographic_map[i + di][j + dj] == cur)
              next_level[{i + di, j + dj}] += rt;
          }
        }
        cur_level = next_level;
      }
      trailhead_scores_sum += cur_level.size();
      for (auto [_, rt] : cur_level) trailhead_ratings_sum += rt;
    }
  }
  std::cout << trailhead_scores_sum << std::endl;
  std::cout << trailhead_ratings_sum << std::endl;

  return 0;
}