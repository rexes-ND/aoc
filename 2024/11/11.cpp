#include <fstream>
#include <iostream>
#include <map>
#include <vector>

long long get_num_stones(
    long long stone, int step,
    std::map<std::pair<long long, int>, long long>& cache) {
  if (cache.count({stone, step})) return cache[{stone, step}];
  if (step == 0) return 1;

  auto stone_str = std::to_string(stone);

  long long res = 0;

  if (stone == 0)
    res = get_num_stones(1, step - 1, cache);
  else if (stone_str.size() % 2 == 0) {
    auto left_stone = std::stoll(stone_str.substr(0, stone_str.size() / 2));
    auto right_stone = std::stoll(
        stone_str.substr(stone_str.size() / 2, stone_str.size() / 2));
    res += get_num_stones(left_stone, step - 1, cache);
    res += get_num_stones(right_stone, step - 1, cache);
  } else
    res = get_num_stones(2024 * stone, step - 1, cache);

  cache[{stone, step}] = res;

  return res;
}

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  int stone;
  std::vector<long long> stones;
  while (input_file >> stone) {
    stones.push_back(stone);
  }

  std::map<std::pair<long long, int>, long long> cache;

  long long num_stones = 0;
  for (auto stone : stones) num_stones += get_num_stones(stone, 25, cache);
  std::cout << num_stones << std::endl;

  num_stones = 0;
  for (auto stone : stones) num_stones += get_num_stones(stone, 75, cache);
  std::cout << num_stones << std::endl;

  return 0;
}
