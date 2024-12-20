#include <fstream>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

std::vector<std::pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  std::string input_line;
  std::vector<std::string> racetrack;
  while (input_file >> input_line) racetrack.push_back(input_line);

  int m = racetrack.size();
  int n = racetrack[0].size();

  auto is_valid = [=](int i, int j) -> bool {
    return 0 <= i && 0 <= j && i < m && j < n;
  };

  std::pair<int, int> start, end;
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      if (racetrack[i][j] == 'S')
        start = {i, j};
      else if (racetrack[i][j] == 'E')
        end = {i, j};
    }
  }

  int curd = 0;
  std::vector<std::vector<int>> dist(m, std::vector(n, -1));
  std::vector<std::pair<int, int>> cur_level = {start};
  while (cur_level.size()) {
    std::vector<std::pair<int, int>> next_level;
    for (auto [i, j] : cur_level) {
      dist[i][j] = curd;
      for (auto [di, dj] : dirs) {
        if (is_valid(i + di, j + dj) && racetrack[i + di][j + dj] != '#' &&
            dist[i + di][j + dj] == -1)
          next_level.push_back({i + di, j + dj});
      }
    }
    curd++;
    cur_level = next_level;
  }
  std::vector<std::pair<int, int>> path;
  std::pair<int, int> cur = end;

  while (cur != start) {
    path.push_back(cur);
    auto [i, j] = cur;
    for (auto [di, dj] : dirs) {
      if (is_valid(i + di, j + dj) && dist[i + di][j + dj] + 1 == dist[i][j]) {
        cur = {i + di, j + dj};
      }
    }
  }
  path.push_back(cur);

  int num_saving_cheats = 0;
  for (auto [i, j] : path) {
    for (auto [di, dj] : dirs) {
      auto ii = i + 2 * di;
      auto jj = j + 2 * dj;
      if (is_valid(ii, jj) && dist[ii][jj] - dist[i][j] - 2 >= 100)
        num_saving_cheats++;
    }
  }
  std::cout << num_saving_cheats << std::endl;

  num_saving_cheats = 0;
  for (auto [i, j] : path) {
    for (auto di = -20; di <= 20; ++di) {
      for (auto dj = -20; dj <= 20; ++dj) {
        if (abs(di) + abs(dj) > 20) continue;
        auto ii = i + di;
        auto jj = j + dj;
        if (is_valid(ii, jj) &&
            dist[ii][jj] - dist[i][j] - abs(di) - abs(dj) >= 100)
          num_saving_cheats++;
      }
    }
  }
  std::cout << num_saving_cheats << std::endl;

  return 0;
}
