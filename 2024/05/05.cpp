#include <fstream>
#include <iostream>
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

  long long sum_middle_page = 0;
  long long sum_corrected_middle_page = 0;
  std::string input_line;
  bool is_empty_line = false;
  std::set<std::pair<int, int>> ordering;
  while (std::getline(input_file, input_line)) {
    if (input_line.size() == 0) {
      is_empty_line = true;
      continue;
    }
    if (!is_empty_line) {
      for (char& c : input_line) {
        if (c == '|') c = ' ';
      }
      int x, y;
      std::stringstream ss(input_line);
      ss >> x >> y;
      ordering.insert({x, y});
    } else {
      for (char& c : input_line) {
        if (c == ',') c = ' ';
      }
      std::stringstream ss(input_line);
      std::vector<int> page_nums;
      int page_num;
      while (ss >> page_num) page_nums.push_back(page_num);
      bool is_correct_order = true;
      for (int i = 0; i < page_nums.size(); ++i) {
        for (int j = i + 1; j < page_nums.size(); ++j) {
          if (ordering.count({page_nums[j], page_nums[i]}))
            is_correct_order = false;
        }
      }
      if (is_correct_order)
        sum_middle_page += page_nums[page_nums.size() / 2];
      else {
        while (!is_correct_order) {
          bool is_swapped = false;
          for (int i = 0; i < page_nums.size(); ++i) {
            for (int j = i + 1; j < page_nums.size(); ++j) {
              if (ordering.count({page_nums[j], page_nums[i]})) {
                std::swap(page_nums[j], page_nums[i]);
                is_swapped = true;
                break;
              }
            }
          }
          if (!is_swapped) is_correct_order = true;
        }
        sum_corrected_middle_page += page_nums[page_nums.size() / 2];
      }
    }
  }
  std::cout << sum_middle_page << std::endl;
  std::cout << sum_corrected_middle_page << std::endl;

  return 0;
}
