#include <cmath>
#include <cstdint>
#include <exception>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::exception;
using std::ifstream;
using std::map;
using std::sqrt;
using std::string;
using std::vector;

class MyException : public exception {
  const char* msg;
public:
  MyException(const char* msg) : msg(msg) {}

  const char* what() const noexcept {
    return msg;
  }
};

typedef int_least64_t OSMID;

struct Node {
  double x, y;
};

struct Road {
  string name;
  vector<OSMID> path;
};

map<OSMID, Node> load_nodes(const string& file_name) {
  ifstream nodes_file(file_name);
  map<OSMID, Node> nodes;
  double min_x, min_y, max_x, max_y;
  if (nodes_file.bad() || nodes_file.fail()) {
    throw MyException("Error opening nodes file.");
  }
  nodes_file >> min_x >> min_y >> max_x >> max_y;
  while (nodes_file.good()) {
    OSMID osmid;
    double x, y;
    nodes_file >> osmid >> x >> y;
    if (nodes_file.good()) {
      nodes[osmid] = {x, y};
    }
  }
  if (nodes_file.bad()) {
    throw MyException("Error reading nodes from file.");
  }
  nodes_file.close();
  return nodes;
}

map<OSMID, Road> load_roads(const string& file_name) {
  map<OSMID, Road> roads;
  ifstream roads_file(file_name);
  long num_roads = 0;
  if (roads_file.bad() || roads_file.fail()) {
    throw MyException("Error opening roads file.");
  }
  while (roads_file.good()) {
    OSMID osmid;
    int path_len;
    roads_file >> osmid >> path_len;
    if (roads_file.good()) {
      Road r;
      r.path.reserve(path_len);
      for (int j = 0; j < path_len; ++j) {
        OSMID node_id;
        roads_file >> node_id;
        ++num_roads;
        r.path.push_back(node_id);
      }
      roads_file.get(); // Read the extra space
      getline(roads_file, r.name);
      roads[osmid] = r;
    }
  }
  if (roads_file.bad()) {
    throw MyException("Error reading roads from file.");
  }
  roads_file.close();
  return roads;
}

double distance(const Node& a, const Node& b) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double get_road_length(const map<OSMID, Node>& nodes, const Road& road) {
  double len = 0.0;
  for (size_t i = 1; i < road.path.size(); ++i) {
    const Node& a = nodes.find(road.path[i - 1])->second;
    const Node& b = nodes.find(road.path[i])->second;
    len += distance(a, b);
  }
  return len;
}

map<string, double> get_road_lengths(
    const map<OSMID, Node>& nodes, const map<OSMID, Road>& roads) {
  map<string, double> road_lengths;
  for (const auto& osmid_road : roads) {
    const Road& road = osmid_road.second;
    road_lengths[road.name] += get_road_length(nodes, road);
  }
  return road_lengths;
}

int main(int argc, char *argv[]) {
  const double min_road_len = 5.0; // km
  map<OSMID, Node> nodes = load_nodes(argc > 1 ? argv[1] : "nodes.txt");
  map<OSMID, Road> roads = load_roads(argc > 2 ? argv[2] : "roads.txt");
  map<string, double> road_lengths = get_road_lengths(nodes, roads);

  cout << "Roads that are at least " << min_road_len << "km:\n";
  for(const auto& name_length : road_lengths) {
    if (name_length.second > min_road_len) {
    cout << "  " << name_length.first << ": " << name_length.second << "km\n";
    }
  }
  cout << endl;
  return 0;
}
