#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  std::string input_line;
  std::vector<u_int64_t> secrets;
  while (input_file >> input_line) {
    secrets.push_back(std::stoll(input_line));
  }

  long long mod = 16777216;
  long long secret_sum = 0;
  std::vector<std::vector<int>> price_changes;
  for (auto secret : secrets) {
    std::vector<int> changes;
    for (int i = 0; i < 2000; ++i) {
      int psd = secret % 10;
      secret = (secret ^ (secret << 6)) % mod;
      secret = (secret ^ (secret >> 5)) % mod;
      secret = (secret ^ (secret << 11)) % mod;
      int sd = secret % 10;
      changes.push_back(sd - psd);
    }
    secret_sum += secret;
    price_changes.push_back(changes);
  }
  std::cout << secret_sum << std::endl;

  std::set<std::tuple<int, int, int, int>> all_seqs;
  std::vector<std::map<std::tuple<int, int, int, int>, int>> seq_to_price(
      secrets.size());
  int most_banana = 0;
  for (auto i = 0; i < secrets.size(); ++i) {
    auto changes = price_changes[i];
    auto& stp = seq_to_price[i];
    auto price = secrets[i] % 10 + changes[0] + changes[1] + changes[2];
    for (int j = 0; j + 3 < 2000; ++j) {
      std::tuple<int, int, int, int> seq = {changes[j], changes[j + 1],
                                            changes[j + 2], changes[j + 3]};
      price += changes[j + 3];
      if (!stp.count(seq)) stp[seq] = price;
      all_seqs.insert(seq);
    }
  }
  for (auto seq : all_seqs) {
    int cand = 0;
    for (int i = 0; i < secrets.size(); ++i) {
      if (seq_to_price[i].count(seq)) {
        cand += seq_to_price[i][seq];
      }
    }
    most_banana = std::max(most_banana, cand);
  }
  std::cout << most_banana << std::endl;

  return -1;
}
