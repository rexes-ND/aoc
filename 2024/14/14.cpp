#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

struct Robot {
  std::pair<long long, long long> init_pos;
  std::pair<long long, long long> cur_pos;
  std::pair<long long, long long> speed;
};

int main() {
  std::ifstream input_file("input.txt");
  if (!input_file) {
    std::cerr << "Error: Unable to open file." << std::endl;
    return -1;
  }

  int wide = 101;
  int tall = 103;
  std::smatch match;
  std::string robot_line;
  std::vector<Robot> robots;
  std::regex robot_pattern("p=(-?\\d+),(-?\\d+) v=(-?\\d+),(-?\\d+)");
  while (std::getline(input_file, robot_line)) {
    std::regex_search(robot_line, match, robot_pattern);
    Robot robot;
    robot.init_pos = {stoi(match[1]), stoi(match[2])};
    robot.cur_pos = robot.init_pos;
    robot.speed = {(stoi(match[3]) % wide + wide) % wide,
                   (stoi(match[4]) % tall + tall) % tall};
    robots.push_back(robot);
  }

  for (int i = 0; i < 100; ++i) {
    for (auto& robot : robots) {
      robot.cur_pos.first += robot.speed.first;
      robot.cur_pos.second += robot.speed.second;
      robot.cur_pos.first %= wide;
      robot.cur_pos.second %= tall;
    }
  }
  std::vector<long long> qs(4, 0);
  for (const auto& robot : robots) {
    int x = robot.cur_pos.first;
    int y = robot.cur_pos.second;
    if (x < wide / 2 && y < tall / 2)
      ++qs[0];
    else if (x > wide / 2 && y < tall / 2)
      ++qs[1];
    else if (x < wide / 2 && y > tall / 2)
      ++qs[2];
    else if (x > wide / 2 && y > tall / 2)
      ++qs[3];
  }

  long long safety_factor = 1;
  for (auto q : qs) safety_factor *= q;
  std::cout << safety_factor << std::endl;

  std::vector<std::pair<int, int>> dirs = {{-1, 0}, {0, 1}, {0, -1}, {1, 0}};
  int max_area_idx = 0;
  int max_area = 0;
  for (int i = 0; i < wide * tall; ++i) {
    for (auto& robot : robots) {
      robot.cur_pos.first = robot.init_pos.first + robot.speed.first * i;
      robot.cur_pos.second = robot.init_pos.second + robot.speed.second * i;
      robot.cur_pos.first %= wide;
      robot.cur_pos.second %= tall;
    }
    std::vector<std::string> pic(tall, std::string(wide, '.'));
    for (auto& robot : robots)
      pic[robot.cur_pos.first][robot.cur_pos.second] = '#';

    std::vector<std::vector<bool>> visit(tall, std::vector<bool>(wide, false));
    for (int y = 0; y < tall; ++y) {
      for (int x = 0; x < wide; ++x) {
        if (pic[y][x] == '.') continue;
        if (visit[y][x]) continue;
        std::vector<std::pair<int, int>> cur_level = {{x, y}};
        visit[y][x] = true;
        int cur_area = 0;
        while (cur_level.size()) {
          cur_area += cur_level.size();
          std::vector<std::pair<int, int>> next_level;
          for (auto [cx, cy] : cur_level) {
            for (auto [dx, dy] : dirs) {
              auto xx = cx + dx;
              auto yy = cy + dy;
              if (xx < 0 || xx >= wide || yy < 0 || yy >= tall ||
                  pic[yy][xx] == '.' || visit[yy][xx])
                continue;
              visit[yy][xx] = true;
              next_level.push_back({xx, yy});
            }
          }
          cur_level = next_level;
        }
        if (cur_area > max_area) {
          max_area = cur_area;
          max_area_idx = i;
        }
      }
    }
  }
  for (auto& robot : robots) {
    robot.cur_pos.first =
        robot.init_pos.first + robot.speed.first * max_area_idx;
    robot.cur_pos.second =
        robot.init_pos.second + robot.speed.second * max_area_idx;
    robot.cur_pos.first %= wide;
    robot.cur_pos.second %= tall;
  }
  std::vector<std::string> pic(tall, std::string(wide, '.'));
  for (auto& robot : robots)
    pic[robot.cur_pos.first][robot.cur_pos.second] = '#';
  for (auto& pic_line : pic) std::cout << pic_line << std::endl;
  std::cout << max_area_idx << std::endl;

  return 0;
}
