#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  std::string input_line;
  std::vector<std::string> maze;
  while (std::getline(input_file, input_line)) maze.push_back(input_line);

  std::pair<int, int> start, end;
  for (int i = 0; i < maze.size(); ++i) {
    for (int j = 0; j < maze[i].size(); ++j) {
      if (maze[i][j] == 'S')
        start = {i, j};
      else if (maze[i][j] == 'E')
        end = {i, j};
    }
  }

  std::vector<std::pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  std::vector<std::vector<std::vector<int>>> dir_dist;
  for (auto dir = 0; dir < dirs.size(); ++dir) {
    std::vector<std::vector<int>> cur;
    for (auto mz : maze) cur.push_back(std::vector<int>(mz.size(), -1));
    dir_dist.push_back(cur);
  }

  std::set<std::tuple<int, int, int, int>> st;
  {
    auto [si, sj] = start;
    for (int dir = 0; dir < dirs.size(); ++dir) {
      int d;
      if (dir == 0 || dir == 2)
        d = 1000;
      else if (dir == 1)
        d = 0;
      else
        d = 2000;
      dir_dist[dir][si][sj] = d;
      st.insert({d, si, sj, dir});
    }
  }

  while (st.size()) {
    auto [d, i, j, dir] = *st.begin();
    st.erase(st.begin());
    auto [di, dj] = dirs[dir];
    i += di;
    j += dj;
    auto& curd = dir_dist[dir][i][j];
    if (curd != -1 && curd < d + 1) continue;
    if (maze[i][j] == '#') continue;
    curd = d + 1;
    st.insert({curd, i, j, dir});
    for (int ddir = 1; ddir < dirs.size(); ++ddir) {
      int nxtd = curd + ((ddir == 2) ? 2000 : 1000);
      int nxtdir = (dir + ddir) % 4;
      int& nxtdd = dir_dist[nxtdir][i][j];
      if (nxtdd == -1 || nxtdd > nxtd) {
        nxtdd = nxtd;
        st.insert({nxtd, i, j, nxtdir});
      }
    }
  }

  int reindeer_score;
  auto [ei, ej] = end;
  reindeer_score = dir_dist[0][ei][ej];
  for (int dir = 1; dir < dirs.size(); ++dir)
    reindeer_score = std::min(reindeer_score, dir_dist[dir][ei][ej]);
  std::cout << reindeer_score << std::endl;

  std::vector<std::vector<bool>> best_path;
  for (auto mz : maze) best_path.push_back(std::vector<bool>(mz.size(), false));
  std::vector<std::tuple<int, int, int>> cur_level;
  for (int dir = 0; dir < dirs.size(); ++dir) {
    if (dir_dist[dir][ei][ej] == reindeer_score) {
      cur_level.push_back({ei, ej, dir});
      best_path[ei][ej] = true;
    }
  }

  while (cur_level.size()) {
    std::vector<std::tuple<int, int, int>> next_level;
    for (auto [i, j, cur_dir] : cur_level) {
      for (int dir = 0; dir < dirs.size(); ++dir) {
        auto [di, dj] = dirs[dir];
        if (maze[i + di][j + dj] == '#') continue;
        int op_dir = (dir + 2) % 4;
        int ddiff = (op_dir - cur_dir + 4) % 4;
        int diff;
        if (ddiff == 0)
          diff = 1;
        else if (ddiff == 2)
          diff = 2001;
        else
          diff = 1001;
        if (dir_dist[op_dir][i + di][j + dj] + diff ==
            dir_dist[cur_dir][i][j]) {
          if (best_path[i + di][j + dj]) continue;
          best_path[i + di][j + dj] = true;
          next_level.push_back({i + di, j + dj, op_dir});
        }
      }
    }
    cur_level = next_level;
  }

  int tile_count = 0;
  for (int i = 0; i < best_path.size(); ++i) {
    for (int j = 0; j < best_path[i].size(); ++j) {
      if (best_path[i][j]) ++tile_count;
    }
  }
  std::cout << tile_count << std::endl;

  return 0;
}
