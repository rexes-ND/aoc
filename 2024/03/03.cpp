#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  bool is_part_two = true;
  bool is_do = true;
  long long result = 0;
  constexpr int mul_expr_len_base = std::strlen("mul(,)");

  std::string do_expr = "do()";
  std::string dont_expr = "don't()";
  std::string input_line;
  while (std::getline(input_file, input_line)) {
    int n = input_line.size();
    for (int i = 0; i < n; ++i) {
      if (is_part_two) {
        if (i + do_expr.size() <= n &&
            input_line.substr(i, do_expr.size()) == do_expr)
          is_do = true;
        else if (i + dont_expr.size() <= n &&
                 input_line.substr(i, dont_expr.size()) == dont_expr)
          is_do = false;
        if (!is_do) continue;
      }
      for (int x_len = 1; x_len <= 3; ++x_len) {
        for (int y_len = 1; y_len <= 3; ++y_len) {
          int mul_expr_len = mul_expr_len_base + x_len + y_len;
          if (i + mul_expr_len > n) continue;
          if (input_line.substr(i, 4) != "mul(") continue;
          if (input_line[i + x_len + 4] != ',') continue;
          if (input_line[i + mul_expr_len - 1] != ')') continue;
          int x_num = 0;
          int y_num = 0;
          bool is_x_valid = true;
          bool is_y_valid = true;
          for (int j = 0; j < x_len; ++j) {
            char x_ch = input_line[i + 4 + j];
            if (!std::isdigit(x_ch)) {
              is_x_valid = false;
              break;
            }
            x_num *= 10;
            x_num += x_ch - '0';
          }
          for (int j = 0; j < y_len; ++j) {
            char y_ch = input_line[i + 5 + x_len + j];
            if (!std::isdigit(y_ch)) {
              is_y_valid = false;
              break;
            }
            y_num *= 10;
            y_num += y_ch - '0';
          }
          if (!is_x_valid || !is_y_valid) continue;
          result += x_num * y_num;
        }
      }
    }
  }
  std::cout << result << std::endl;

  return 0;
}