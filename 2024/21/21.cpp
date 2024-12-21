#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <tuple>
#include <vector>

std::vector<std::pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
std::map<char, std::pair<int, int>> char_to_dir = {
    {'^', {-1, 0}}, {'>', {0, 1}}, {'v', {1, 0}}, {'<', {0, -1}}};
std::map<std::pair<int, int>, char> dir_to_char = {
    {{-1, 0}, '^'}, {{0, -1}, '<'}, {{1, 0}, 'v'}, {{0, 1}, '>'}};

std::set<std::string> bfs(char start, char end,
                          const std::vector<std::string>& keypad) {
  int m = keypad.size();
  int n = keypad[0].size();
  std::vector<std::vector<int>> dist(m, std::vector(n, -1));
  std::vector<std::vector<std::set<std::string>>> seqs(
      m, std::vector<std::set<std::string>>(n));
  int si, sj;
  int ei, ej;
  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j) {
      if (keypad[i][j] == start) {
        si = i;
        sj = j;
      }
      if (keypad[i][j] == end) {
        ei = i;
        ej = j;
      }
    }
  auto is_valid = [=](int i, int j) {
    return 0 <= i && i < m && 0 <= j && j < n;
  };
  int curd = 0;
  std::vector<std::pair<int, int>> cur_level = {{si, sj}};
  seqs[si][sj] = {""};
  while (cur_level.size()) {
    std::vector<std::pair<int, int>> next_level;
    for (auto [i, j] : cur_level) dist[i][j] = curd;
    for (auto [i, j] : cur_level) {
      for (auto [di, dj] : dirs) {
        auto ii = i + di;
        auto jj = j + dj;
        if (!is_valid(ii, jj)) continue;
        if (dist[ii][jj] != -1) continue;
        if (keypad[ii][jj] == '#') continue;
        for (auto p : seqs[i][j]) {
          p.push_back(dir_to_char[{di, dj}]);
          seqs[ii][jj].insert(p);
        }
        next_level.push_back({ii, jj});
      }
    }
    ++curd;
    cur_level = next_level;
  }
  return seqs[ei][ej];
}

std::map<std::tuple<char, char, int>, long long> cache;

struct Keypad {
  std::vector<std::string> keypad;
  std::map<std::pair<char, char>, std::set<std::string>> shortest_seqs;
  long long get_seq_size(char c1, char c2, int lvl, Keypad& dk) {
    long long seq_size = 0;
    for (auto shortest_seq : shortest_seqs[{c1, c2}]) {
      long long cur_seq_size = 0;
      char prev_ch = 'A';
      for (char ch : shortest_seq) {
        cur_seq_size += dk.get_seq_size(prev_ch, ch, lvl);
        prev_ch = ch;
      }
      if (seq_size == 0 || seq_size > cur_seq_size) seq_size = cur_seq_size;
    }
    return seq_size;
  }
  long long get_seq_size(char c1, char c2, int lvl) {
    if (cache.count({c1, c2, lvl})) return cache[{c1, c2, lvl}];
    long long seq_size = 0;
    for (auto shortest_seq : shortest_seqs[{c1, c2}]) {
      char prev_ch = 'A';
      long long cur_seq_size = 0;
      for (char ch : shortest_seq) {
        cur_seq_size += get_seq_size(prev_ch, ch, lvl - 1);
        prev_ch = ch;
      }
      if (seq_size == 0 || seq_size > cur_seq_size) seq_size = cur_seq_size;
    }
    cache[{c1, c2, lvl}] = seq_size;
    return seq_size;
  }
  long long get_complexity(std::string code, Keypad& dk, int lvl) {
    long long seq_size = 0;
    char prev_ch = 'A';
    for (char ch : code) {
      seq_size += get_seq_size(prev_ch, ch, lvl, dk);
      prev_ch = ch;
    }
    code.pop_back();
    int num = std::stoi(code);
    return seq_size * num;
  }
  Keypad(std::vector<std::string> keypad, bool init_cache = false) {
    this->keypad = keypad;
    std::set<char> chs;
    for (int i = 0; i < keypad.size(); ++i) {
      for (int j = 0; j < keypad[i].size(); ++j) {
        char ch = keypad[i][j];
        if (ch == '#') continue;
        chs.insert(ch);
      }
    }
    for (char ch1 : chs) {
      for (char ch2 : chs) {
        for (auto shortest_seq : bfs(ch1, ch2, keypad)) {
          shortest_seq.push_back('A');
          shortest_seqs[{ch1, ch2}].insert(shortest_seq);
          if (init_cache) cache[{ch1, ch2, 1}] = shortest_seq.size();
        }
      }
    }
  }
};

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  std::string input_line;
  std::vector<std::string> codes;
  while (input_file >> input_line) codes.push_back(input_line);

  Keypad nk({"789", "456", "123", "#0A"});
  Keypad dk({"#^A", "<v>"}, true);

  std::vector<int> lvls = {2, 25};
  for (auto lvl : lvls) {
    long long complexity_sum = 0;
    for (auto code : codes) complexity_sum += nk.get_complexity(code, dk, lvl);
    std::cout << complexity_sum << std::endl;
  }

  return 0;
}
