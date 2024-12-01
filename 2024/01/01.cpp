#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  int x, y;
  std::vector<int> first_list;
  std::vector<int> second_list;
  while (input_file >> x >> y) {
    first_list.push_back(x);
    second_list.push_back(y);
  }
  sort(first_list.begin(), first_list.end());
  sort(second_list.begin(), second_list.end());

  if (first_list.size() != second_list.size()) {
    std::cerr << "Error: The size of the 2 lists doesn't match" << std::endl;
    return -1;
  }

  long long total_distance = 0;
  for (int i = 0; i < first_list.size(); ++i) {
    total_distance += abs(first_list[i] - second_list[i]);
  }
  std::cout << total_distance << std::endl;

  std::unordered_map<int, int> freq_count;
  for (int y : second_list) freq_count[y]++;
  long long similarity_score = 0;
  for (int x : first_list) {
    similarity_score += x * freq_count[x];
  }
  std::cout << similarity_score << std::endl;

  return 0;
}
