#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

struct Device {
  std::vector<int> xwires;
  std::vector<int> ywires;

  // wire3 <= wire1, wire2, gate
  std::map<std::string, std::tuple<std::string, std::string, std::string>> deps;
  std::map<std::string, int> cache;

  int get_wire(std::string w) {
    if (w[0] == 'x') return xwires[10 * (w[1] - '0') + (w[2] - '0')];
    if (w[0] == 'y') return ywires[10 * (w[1] - '0') + (w[2] - '0')];
    if (cache.count(w)) return cache[w];
    auto [w1, w2, g] = deps[w];
    int val;
    if (g == "AND")
      val = get_wire(w1) & get_wire(w2);
    else if (g == "OR")
      val = get_wire(w1) | get_wire(w2);
    else
      val = get_wire(w1) ^ get_wire(w2);
    cache[w] = val;
    return val;
  }
};

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  Device dev;
  std::string input_line;
  bool is_empty = false;
  while (std::getline(input_file, input_line)) {
    if (input_line.size() == 0) {
      is_empty = true;
      continue;
    }
    if (is_empty) {
      std::stringstream ss(input_line);
      std::string w1, w2, gate, arrow, w3;
      ss >> w1 >> gate >> w2 >> arrow >> w3;
      if (w1 > w2) std::swap(w1, w2);
      dev.deps[w3] = {w1, w2, gate};
    } else {
      for (char& c : input_line)
        if (c == ':') c = ' ';
      std::stringstream ss(input_line);
      std::string wire;
      int val;
      ss >> wire >> val;
      if (wire[0] == 'x')
        dev.xwires.push_back(val);
      else
        dev.ywires.push_back(val);
    }
  }
  uint64_t num = 0;
  for (uint64_t i = 0; i <= dev.xwires.size(); ++i) {
    std::string z = "z";
    if (i < 10) z.push_back('0');
    z += std::to_string(i);
    num += (1ULL << i) * dev.get_wire(z);
  }
  std::cout << num << std::endl;

  std::swap(dev.deps["fgc"], dev.deps["z12"]);
  std::swap(dev.deps["mtj"], dev.deps["z29"]);
  std::swap(dev.deps["vvm"], dev.deps["dgr"]);
  std::swap(dev.deps["dtv"], dev.deps["z37"]);
  std::map<std::pair<std::string, std::string>, std::string> xors, ands, ors;
  for (auto [w3, val] : dev.deps) {
    auto [w1, w2, gate] = val;
    if (w1 > w2) swap(w1, w2);
    if (gate == "XOR")
      xors[{w1, w2}] = w3;
    else if (gate == "AND")
      ands[{w1, w2}] = w3;
    else
      ors[{w1, w2}] = w3;
  }

  std::string carry = ands[{"x00", "y00"}];
  for (int i = 1; i < 45; ++i) {
    std::cout << "c" << i << ' ' << carry << std::endl;
    std::string x = "x", y = "y";
    if (i < 10) {
      x.push_back('0');
      y.push_back('0');
    }
    x += std::to_string(i);
    y += std::to_string(i);
    std::string and_xy, xor_xy;
    and_xy = ands[{x, y}];
    xor_xy = xors[{x, y}];
    std::cout << "xor " << x << ' ' << y << ": " << xor_xy << std::endl;
    std::cout << "and " << x << ' ' << y << ": " << and_xy << std::endl;
    std::string z;

    std::string xor_xy_c = xor_xy;
    std::string and_xy_c = and_xy;
    std::string carry_c = carry;
    if (xor_xy_c > carry_c) std::swap(xor_xy_c, carry_c);
    z = xors[{xor_xy_c, carry_c}];
    std::cout << z << std::endl;
    xor_xy_c = xor_xy;
    carry_c = carry;
    if (xor_xy_c > carry_c) std::swap(xor_xy_c, carry_c);
    std::string xor_and_carry = ands[{xor_xy_c, carry_c}];
    std::cout << "xor_and_carry " << xor_and_carry << std::endl;
    if (and_xy_c > xor_and_carry) std::swap(and_xy_c, xor_and_carry);
    carry = ors[{and_xy_c, xor_and_carry}];
    std::cout << std::endl;
  }

  std::vector<std::string> swapped_wires = {"fgc", "z12", "mtj", "z29",
                                            "vvm", "dgr", "dtv", "z37"};
  std::sort(swapped_wires.begin(), swapped_wires.end());
  std::string swapped_wires_str = swapped_wires[0];
  for (int i = 1; i < 8; ++i) swapped_wires_str += "," + swapped_wires[i];
  std::cout << swapped_wires_str << std::endl;

  return 0;
}
