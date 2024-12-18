#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  std::vector<std::pair<int, int>> coords;
  std::string input_line;
  while (input_file >> input_line) {
    for (auto& c : input_line)
      if (c == ',') c = ' ';
    std::stringstream ss(input_line);
    int X, Y;
    ss >> X >> Y;
    coords.push_back({X, Y});
  }

  std::vector<std::string> memmap(71, std::string(71, '.'));
  int i = 0;
  for (; i < 1024; ++i) {
    auto [X, Y] = coords[i];
    memmap[Y][X] = '#';
  }
  std::vector<std::vector<int>> dist(71, std::vector(71, -1));
  std::set<std::tuple<int, int, int>> st;
  std::vector<std::pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  st.insert({0, 0, 0});
  while (st.size()) {
    auto [d, i, j] = *st.begin();
    st.erase(st.begin());
    if (dist[i][j] != -1 && dist[i][j] <= d) continue;
    dist[i][j] = d;
    for (auto [di, dj] : dirs) {
      auto ii = i + di;
      auto jj = j + dj;
      if (std::min(ii, jj) < 0 || std::max(ii, jj) >= 71 ||
          memmap[ii][jj] == '#')
        continue;
      st.insert({d + 1, ii, jj});
    }
  }

  std::cout << dist[70][70] << std::endl;

  while (true) {
    auto [X, Y] = coords[i];
    memmap[Y][X] = '#';
    for (int i = 0; i <= 70; ++i)
      for (int j = 0; j <= 70; ++j) dist[i][j] = -1;
    st.clear();
    st.insert({0, 0, 0});
    while (st.size()) {
      auto [d, i, j] = *st.begin();
      st.erase(st.begin());
      if (dist[i][j] != -1 && dist[i][j] <= d) continue;
      dist[i][j] = d;
      for (auto [di, dj] : dirs) {
        auto ii = i + di;
        auto jj = j + dj;
        if (std::min(ii, jj) < 0 || std::max(ii, jj) >= 71 ||
            memmap[ii][jj] == '#')
          continue;
        st.insert({d + 1, ii, jj});
      }
    }

    if (dist[70][70] == -1)
      break;
    else
      ++i;
  }

  std::cout << coords[i].first << ',' << coords[i].second << std::endl;

  return 0;
}
