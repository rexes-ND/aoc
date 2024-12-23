#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
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

  std::set<std::string> coms;
  std::map<std::string, std::set<std::string>> conns;
  std::string input_line;
  while (input_file >> input_line) {
    for (char& c : input_line)
      if (c == '-') c = ' ';
    std::stringstream ss(input_line);
    std::string c1, c2;
    ss >> c1 >> c2;
    conns[c1].insert(c2);
    conns[c2].insert(c1);
    coms.insert(c1);
    coms.insert(c2);
  }

  int num_com_with_t = 0;
  for (auto c1 : coms) {
    for (auto c2 : coms) {
      if (c1 >= c2 || !conns[c1].count(c2)) continue;
      for (auto c3 : conns[c1]) {
        if (c2 >= c3 || !conns[c2].count(c3)) continue;
        if (c1[0] == 't' || c2[0] == 't' || c3[0] == 't') ++num_com_with_t;
      }
    }
  }
  std::cout << num_com_with_t << std::endl;

  std::set<int> cnts;
  for (auto c : coms) cnts.insert(conns[c].size());
  assert(cnts.size() == 1);
  int cnt = *cnts.begin();

  int max_sz = 1;
  std::string lan_party = *coms.begin();
  for (auto c : coms) {
    std::vector<std::string> connsv = {conns[c].begin(), conns[c].end()};
    for (int i = 0; i < (1 << cnt); ++i) {
      std::vector<std::string> sub_connsv;
      for (int j = 0; j < cnt; ++j) {
        if (i & (1 << j)) sub_connsv.push_back(connsv[j]);
      }
      auto pos = true;
      for (int j = 0; j < sub_connsv.size(); ++j) {
        for (int k = j + 1; k < sub_connsv.size(); ++k) {
          if (!conns[sub_connsv[j]].count(sub_connsv[k])) pos = false;
        }
      }
      if (pos && sub_connsv.size() >= max_sz) {
        max_sz = sub_connsv.size() + 1;
        sub_connsv.push_back(c);
        std::sort(sub_connsv.begin(), sub_connsv.end());
        lan_party = sub_connsv[0];
        for (int j = 1; j < sub_connsv.size(); ++j)
          lan_party += "," + sub_connsv[j];
      }
    }
  }
  std::cout << lan_party << std::endl;

  return 0;
}
