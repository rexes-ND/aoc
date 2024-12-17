#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Computer {
  int instr_ptr;
  long long reg_a;
  long long reg_b;
  long long reg_c;
  std::string output;
  std::vector<int> output_vec;
  std::vector<std::pair<int, int>> prog;
  long long operand_to_val(int op) {
    switch (op) {
      case 0:
      case 1:
      case 2:
      case 3:
        return op;
      case 4:
        return reg_a;
      case 5:
        return reg_b;
      case 6:
        return reg_c;
      case 7:
        throw std::runtime_error("7 should be reserved!");
      default:
        throw std::runtime_error("impossible case!");
    }
  }
  bool exec() {
    auto [opcode, operand] = prog[instr_ptr];
    auto val = operand_to_val(operand);
    instr_ptr++;
    switch (opcode) {
      case 0:
        reg_a >>= val;
        break;
      case 1:
        reg_b ^= operand;
        break;
      case 2:
        reg_b = val & 7;
        break;
      case 3:
        if (reg_a != 0) {
          instr_ptr = operand >> 1;
        }
        break;
      case 4:
        reg_b ^= reg_c;
        break;
      case 5:
        output += std::to_string(val & 7);
        output_vec.push_back(val & 7);
        output.push_back(',');
        break;
      case 6:
        reg_b = reg_a >> val;
        break;
      case 7:
        reg_c = reg_a >> val;
        break;
    }
    return instr_ptr < prog.size();
  }
  void reset(long long ra, long long rb, long long rc) {
    instr_ptr = 0;
    reg_a = ra;
    reg_b = rb;
    reg_c = rc;
    output.clear();
    output_vec.clear();
  }
};

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  const int reg_prefix_len = std::strlen("Register A: ");
  const int prog_prefix_len = std::strlen("Program: ");
  std::string reg_a_line, reg_b_line, reg_c_line, prog_line;
  std::getline(input_file, reg_a_line);
  std::getline(input_file, reg_b_line);
  std::getline(input_file, reg_c_line);
  std::getline(input_file, prog_line);
  std::getline(input_file, prog_line);

  long long reg_a = std::stoll(reg_a_line.substr(reg_prefix_len));
  long long reg_b = std::stoll(reg_b_line.substr(reg_prefix_len));
  long long reg_c = std::stoll(reg_c_line.substr(reg_prefix_len));

  prog_line = prog_line.substr(prog_prefix_len);
  for (char& c : prog_line)
    if (c == ',') c = ' ';

  std::string num;
  std::stringstream prog_stream(prog_line);
  std::vector<std::pair<int, int>> prog;
  std::vector<int> prog_vec;
  while (prog_stream >> num) {
    std::pair<int, int> instr;
    instr.first = stoi(num);
    prog_stream >> num;
    instr.second = stoi(num);
    prog.push_back(instr);
    prog_vec.push_back(instr.first);
    prog_vec.push_back(instr.second);
  }

  Computer computer;
  computer.instr_ptr = 0;
  computer.reg_a = reg_a;
  computer.reg_b = reg_b;
  computer.reg_c = reg_c;
  computer.prog = prog;

  while (computer.exec());
  if (computer.output.size()) computer.output.pop_back();
  std::cout << computer.output << std::endl;

  std::reverse(prog_vec.begin(), prog_vec.end());
  std::vector<long long> cur_level = {0};
  for (int ind = 0; ind < prog_vec.size(); ++ind) {
    std::vector<long long> next_level;
    for (auto cur_ra : cur_level) {
      for (int i = 0; i < 8; ++i) {
        auto next_ra = 8 * cur_ra + i;
        computer.reset(next_ra, reg_b, reg_c);
        while (computer.output_vec.size() == 0) computer.exec();
        if (computer.output_vec[0] == prog_vec[ind])
          next_level.push_back(next_ra);
      }
    }
    cur_level = next_level;
  }
  std::cout << cur_level[0] << std::endl;

  return 0;
}
