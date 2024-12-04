#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }
  const std::string xmas = "XMAS";
  std::vector<std::pair<int, int>> dirs = {{1, 0}, {-1, 0}, {0, 1},  {0, -1},
                                           {1, 1}, {-1, 1}, {1, -1}, {-1, -1}};

  std::string input_line;
  std::vector<std::string> input_lines;
  while (std::getline(input_file, input_line))
    input_lines.push_back(input_line);

  int xmas_appearance = 0;
  for (int i = 0; i < input_lines.size(); ++i) {
    for (int j = 0; j < input_lines[i].size(); ++j) {
      for (auto [di, dj] : dirs) {
        int ii = i;
        int jj = j;
        bool found = true;
        for (int k = 0; k < xmas.size(); ++k) {
          if (ii < 0 || ii >= input_lines.size() || jj < 0 ||
              jj >= input_lines[ii].size() || input_lines[ii][jj] != xmas[k]) {
            found = false;
            break;
          }
          ii += di;
          jj += dj;
        }
        if (found) xmas_appearance++;
      }
    }
  }
  std::cout << xmas_appearance << std::endl;

  std::set<std::string> x_mas_set = {"MMSS", "SMMS", "SSMM", "MSSM"};
  dirs = {{-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
  int x_mas_appearance = 0;
  for (int i = 0; i < input_lines.size(); ++i) {
    for (int j = 0; j < input_lines[i].size(); ++j) {
      if (input_lines[i][j] != 'A') continue;
      std::string cand_x_mas;
      for (auto [di, dj] : dirs) {
        int ii = i + di;
        int jj = j + dj;
        if (ii < 0 || ii >= input_lines.size() || jj < 0 ||
            jj >= input_lines[ii].size())
          break;
        cand_x_mas.push_back(input_lines[ii][jj]);
      }
      if (x_mas_set.count(cand_x_mas)) ++x_mas_appearance;
    }
  }
  std::cout << x_mas_appearance << std::endl;

  return 0;
}
