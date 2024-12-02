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

  std::vector<std::vector<int>> reports;

  std::string report_line;
  while (std::getline(input_file, report_line)) {
    std::vector<int> report;
    std::istringstream report_stream(report_line);
    int report_num;
    while (report_stream >> report_num) report.push_back(report_num);
    reports.push_back(report);
  }

  int num_safe_reports = 0;
  for (auto& report : reports) {
    bool is_inc = true;
    bool is_dec = true;
    bool is_diff = true;
    for (int i = 0; i < report.size() - 1; ++i) {
      int diff = abs(report[i + 1] - report[i]);
      is_diff = is_diff && diff >= 1 && diff <= 3;
      is_inc = is_inc && report[i] < report[i + 1];
      is_dec = is_dec && report[i] > report[i + 1];
    }
    if (is_diff && (is_inc || is_dec)) ++num_safe_reports;
  }
  std::cout << num_safe_reports << std::endl;

  num_safe_reports = 0;
  for (auto report : reports) {
    for (int i = 0; i < report.size(); ++i) {
      bool is_inc = true;
      bool is_dec = true;
      bool is_diff = true;
      auto rreport = report;
      rreport.erase(rreport.begin() + i);
      for (int j = 0; j < rreport.size() - 1; ++j) {
        int diff = abs(rreport[j + 1] - rreport[j]);
        is_diff = is_diff && diff >= 1 && diff <= 3;
        is_inc = is_inc && rreport[j] < rreport[j + 1];
        is_dec = is_dec && rreport[j] > rreport[j + 1];
      }
      if (is_diff && (is_inc || is_dec)) {
        ++num_safe_reports;
        break;
      }
    }
  }
  std::cout << num_safe_reports << std::endl;

  return 0;
}