#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  std::string equation;
  std::vector<std::string> equations;
  while (std::getline(input_file, equation)) equations.push_back(equation);

  long long calibration_result = 0;
  long long concat_calibration_result = 0;
  for (auto equation : equations) {
    for (auto& c : equation)
      if (c == ':') c = ' ';

    std::stringstream ss(equation);
    long long test_value;
    ss >> test_value;
    int num;
    std::vector<int> nums;
    while (ss >> num) nums.push_back(num);
    bool pos = false;
    bool concat_pos = false;
    for (int i = 0; i < (1 << (nums.size() - 1)); ++i) {
      long long value = nums[0];
      for (int j = 0; j < nums.size() - 1; ++j) {
        if (i & (1 << j)) {
          value += nums[j + 1];
        } else {
          value *= nums[j + 1];
        }
      }
      if (value == test_value) {
        pos = true;
        break;
      }
    }
    if (pos) {
      calibration_result += test_value;
      concat_calibration_result += test_value;
      continue;
    }

    int all_pos = std::pow(3, nums.size() - 1);
    for (int i = 0; i < all_pos; ++i) {
      long long value = nums[0];
      int cur_i = i;
      for (int j = 0; j < nums.size() - 1; ++j) {
        if (cur_i % 3 == 0) {
          value += nums[j + 1];
        } else if (cur_i % 3 == 1) {
          value *= nums[j + 1];
        } else {
          value =
              std::stoll(std::to_string(value) + std::to_string(nums[j + 1]));
        }
        cur_i /= 3;
      }
      if (value == test_value) {
        concat_pos = true;
        break;
      }
    }
    if (concat_pos) {
      concat_calibration_result += test_value;
    }
  }

  std::cout << calibration_result << std::endl;
  std::cout << concat_calibration_result << std::endl;

  return 0;
}
