#include <fstream>
#include <iostream>
#include <vector>

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  std::string disk_map;
  std::getline(input_file, disk_map);

  std::vector<int> less_dense_disk_map;
  for (int i = 0; i < disk_map.size(); ++i) {
    int num = disk_map[i] - '0';
    if (i & 1) {
      for (int j = 0; j < num; ++j) less_dense_disk_map.push_back(-1);
    } else {
      for (int j = 0; j < num; ++j) less_dense_disk_map.push_back(i / 2);
    }
  }

  std::vector<int> moved_disk_map = less_dense_disk_map;
  int i = 0;
  int j = moved_disk_map.size() - 1;
  while (true) {
    while (i < moved_disk_map.size() && moved_disk_map[i] != -1) ++i;
    while (j >= 0 && moved_disk_map[j] == -1) --j;
    if (i < j) {
      std::swap(moved_disk_map[i], moved_disk_map[j]);
      i++;
      j--;
    } else
      break;
  }

  long long filesystem_checksum = 0;
  for (int pos = 0; pos < moved_disk_map.size(); ++pos)
    if (moved_disk_map[pos] != -1)
      filesystem_checksum += pos * moved_disk_map[pos];
  std::cout << filesystem_checksum << std::endl;

  std::vector<int> frag_moved_disk_map = less_dense_disk_map;
  j = frag_moved_disk_map.size() - 1;
  while (true) {
    int i = 0;
    while (j >= 0 && frag_moved_disk_map[j] == -1) --j;
    int jj = j;
    while (jj - 1 >= 0 &&
           frag_moved_disk_map[jj - 1] == frag_moved_disk_map[jj])
      --jj;
    int j_len = j - jj + 1;
    if (j <= 0) break;
    bool found = false;
    while (true) {
      while (i < frag_moved_disk_map.size() && frag_moved_disk_map[i] != -1)
        ++i;
      if (i < j) {
        // potential cand;
        int ii = i;
        while (ii + 1 < frag_moved_disk_map.size() &&
               frag_moved_disk_map[ii + 1] == -1)
          ++ii;
        int i_len = ii - i + 1;
        if (i_len >= j_len) {
          found = true;
          break;
        } else
          i = ii + 1;
      } else
        break;
    }
    if (found) {
      for (int k = 0; k < j_len; ++k) {
        std::swap(frag_moved_disk_map[i + k], frag_moved_disk_map[j - k]);
      }
    }
    j -= j_len;
  }

  long long frag_filesystem_checksum = 0;

  for (int pos = 0; pos < frag_moved_disk_map.size(); ++pos) {
    if (frag_moved_disk_map[pos] != -1)
      frag_filesystem_checksum += pos * frag_moved_disk_map[pos];
  }
  std::cout << frag_filesystem_checksum << std::endl;

  return 0;
}