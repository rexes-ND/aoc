#include <fstream>
#include <iostream>
#include <set>
#include <vector>

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  std::string input_line;
  std::vector<std::string> garden_plots;
  std::vector<std::vector<bool>> visit;

  while (std::getline(input_file, input_line)) {
    garden_plots.push_back(input_line);
    visit.push_back(std::vector<bool>(input_line.size(), false));
  }

  std::vector<std::pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

  long long total_price = 0;
  long long discounted_price = 0;
  for (int i = 0; i < garden_plots.size(); ++i) {
    auto line = garden_plots[i];
    for (int j = 0; j < line.size(); ++j) {
      if (visit[i][j]) continue;
      char plot = line[j];
      std::set<std::pair<int, int>> region;
      std::vector<std::pair<int, int>> cur_level = {{i, j}};
      while (cur_level.size()) {
        std::vector<std::pair<int, int>> next_level;
        for (auto [i, j] : cur_level) {
          region.insert({i, j});
          for (auto [di, dj] : dirs) {
            auto ii = i + di;
            auto jj = j + dj;
            if (ii < 0 || ii >= garden_plots.size() || jj < 0 ||
                jj >= line.size() || garden_plots[ii][jj] != plot ||
                visit[ii][jj])
              continue;
            visit[ii][jj] = true;
            next_level.push_back({ii, jj});
          }
        }
        cur_level = next_level;
      }
      int area = region.size();
      int perimeter = 0;
      int sides = 0;
      std::vector<std::set<std::pair<int, int>>> ds(dirs.size());
      for (auto [i, j] : region) {
        for (int k = 0; k < dirs.size(); ++k) {
          auto [di, dj] = dirs[k];
          auto ii = i + di;
          auto jj = j + dj;
          if (region.count({ii, jj})) continue;
          perimeter++;
          ds[k].insert((di == 0) ? std::make_pair(j, i) : std::make_pair(i, j));
        }
      }
      for (int k = 0; k < dirs.size(); ++k) {
        auto cur = std::make_pair(-1, -1);
        for (auto p : ds[k]) {
          if (cur.first < p.first || cur.second + 1 < p.second) ++sides;
          cur = p;
        }
      }
      total_price += 1LL * area * perimeter;
      discounted_price += 1LL * area * sides;
    }
  }
  std::cout << total_price << std::endl;
  std::cout << discounted_price << std::endl;

  return 0;
}
