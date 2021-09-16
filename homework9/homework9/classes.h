#pragma once
#include <cmath>
#include <cstdint>
#include <exception>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <unordered_map>

class MyException : public std::exception {
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
    std::string name;
    std::vector<OSMID> path;
};

struct Edge {
    OSMID road_id, node_a_id, node_b_id;
    double weight;
    Edge(OSMID road_id, OSMID node_x_id, OSMID node_y_id, double weight) : road_id(road_id), weight(weight) {
        node_a_id = node_x_id;
        node_b_id = node_y_id;
        if (false && node_x_id > node_y_id) {
            node_a_id = node_y_id;
            node_b_id = node_x_id;
        }
    }
    bool operator<(const Edge& rhs) const noexcept {
        return (weight > rhs.weight);
    }
    bool operator>(const Edge& rhs) const noexcept {
        return (*this < rhs);
    }
    bool operator==(const Edge& rhs) const noexcept {
        return (road_id == rhs.road_id &&
            node_a_id == rhs.node_a_id &&
            node_b_id == rhs.node_b_id &&
            weight == rhs.weight);
    }
};
class NodeUnion { 
    std::unordered_map<OSMID, OSMID> parent;
public:
    void make_set(std::map<OSMID, Node> const& possible_nodes) {
        for (auto& elem : possible_nodes) {
            OSMID i = elem.first;
            parent[i] = i;
        }
    }
    OSMID find(OSMID x) { 
        if (parent[x] == x) {
            return x;
        }
        return find(parent[x]);
    }
    void Union(OSMID a, OSMID b) { 
        OSMID x = find(a);
        OSMID y = find(b);
        parent[x] = y;
    }
};
typedef  std::pair<OSMID, OSMID> NodePair;


class RoadFinder {
private:

public:
    std::map<OSMID, Node> nodes;
    std::map<OSMID, Road> roads;
    std::priority_queue<Edge> q_edges;
    std::set<Edge> min_edges;

    Node& get_node(std::map<OSMID, Node>& nodes, const OSMID id) {
        return nodes[id];
    }
    Node& get_node(const OSMID id) {
        return get_node(nodes, id);
    }
    std::map<OSMID, Node> & load_nodes(const std::string& file_name, std::map<OSMID, Node> &nodes) {
        std::ifstream nodes_file(file_name);
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
                nodes[osmid] = { x, y };
            }
        }
        if (nodes_file.bad()) {
            throw MyException("Error reading nodes from file.");
        }
        nodes_file.close();
        return nodes;
    }
    std::map<OSMID, Node> &load_nodes(const std::string& file_name) {
        return load_nodes(file_name, nodes);
    }
    std::map<OSMID, Node> &load_nodes() {
        return load_nodes("nodes.txt");
    }
    std::map<OSMID, Road>& load_roads(const std::string& file_name, std::map<OSMID, Road> &roads) {
        
        std::ifstream roads_file(file_name);
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
                std::getline(roads_file, r.name);
                roads[osmid] = r;
            }
        }
        if (roads_file.bad()) {
            throw MyException("Error reading roads from file.");
        }
        roads_file.close();
        return roads;
    }
    std::map<OSMID, Road> & load_roads(const std::string& file_name) {
        return load_roads(file_name, roads);
    }
    std::map<OSMID, Road>& load_roads() {
        return load_roads("roads.txt");
    }
    double distance(const Node& a, const Node& b) {
        return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }
    double distance(const OSMID x, const OSMID y) {
        const Node & a = get_node(x);
        const Node & b = get_node(y);
        return distance(a, b);
    }
    double get_road_length(const std::map<OSMID, Node>& nodes, const Road& road) {
        double len = 0.0;
        for (size_t i = 1; i < road.path.size(); ++i) {
            const Node& a = nodes.find(road.path[i - 1])->second;
            const Node& b = nodes.find(road.path[i])->second;
            len += distance(a, b);
        }
        return len;
    }

    std::map<std::string, double> get_road_lengths(
        const std::map<OSMID, Node>& nodes, const std::map<OSMID, Road>& roads) {
        std::map<std::string, double> road_lengths;
        for (const auto& osmid_road : roads) {
            const Road& road = osmid_road.second;
            road_lengths[road.name] += get_road_length(nodes, road);
        }
        return road_lengths;
    }

    std::priority_queue<Edge>& load_edges(const std::map<OSMID, Node>& nodes, const std::map<OSMID, Road>& roads, std::priority_queue<Edge>& edges) {
        for (auto& road : roads) {
            for (auto it = road.second.path.begin(); it != road.second.path.end(); ++it) {
                auto next_it = it + 1;
                if (next_it != road.second.path.end()) {
                    Edge e(road.first, *it, *next_it, distance(*it, *next_it));
                    edges.push(e);
                }
            }
        }
        return edges;
    }

    std::priority_queue<Edge>& load_edges() {
        return load_edges(nodes, roads, q_edges);
    }
    template <typename T>
    bool exists_in_vector(std::vector<T>& input, T& object) {
        return (std::find(input.begin(), input.end(), object) == input.end());
    }

    bool nodes_connected_old(const std::set<Edge>& edges, const Edge &edge) {
        std::vector<Edge> path;
        path.push_back(edge);
        bool a_found = false;
        bool b_found = false;
        while (!path.empty()) {
            //find all edges with new node
            auto & cur_path = path.back();
            for (auto elem : edges) {
                if (elem.node_a_id == cur_path.node_a_id || elem.node_b_id == cur_path.node_a_id) {
                    a_found = true;
                }
                else if (elem.node_a_id == cur_path.node_b_id || elem.node_b_id == cur_path.node_b_id) {
                    b_found = true;
                }
                if (a_found && b_found) {
                    return true;
                }
            }
            path.pop_back();
        }
        return false;
    }
    bool nodes_connected_depth(const std::map<OSMID, std::set<Edge>>& map_edges, const Edge& edge) {
        std::vector<Edge> v_edges;
        OSMID start_id = edge.node_a_id;
        OSMID destination_id = edge.node_b_id;
        //This code is extra if we already do the check for new nodes, it stops the bottom from erroring if that code is removed
        auto it_cur_map = map_edges.find(start_id);
        if (it_cur_map == map_edges.end()) { 
            start_id = edge.node_b_id;
            destination_id = edge.node_a_id;
            it_cur_map = map_edges.find(start_id);
            if (it_cur_map == map_edges.end()) {
                return false;
            }
        }
        std::set<OSMID> searched_nodes;
        searched_nodes.emplace(start_id);
        for (auto & elem : it_cur_map->second) {
            v_edges.emplace_back(elem);
        }
        while(!v_edges.empty()){
            auto cur_edge = v_edges.back();
            if (cur_edge.node_a_id == destination_id || cur_edge.node_b_id == destination_id) {
                return true; //not triggering as often as it should :(
            }
            //check if we have already added all the edges for the current nodes.
            if (searched_nodes.find(cur_edge.node_a_id) == searched_nodes.end()) {
                auto it_cur_map = map_edges.find(cur_edge.node_a_id);
                if (it_cur_map != map_edges.end()) {
                    for (auto& cur_set_edge : it_cur_map->second) {
                        v_edges.emplace_back(cur_set_edge);
                    }
                }
                searched_nodes.emplace(cur_edge.node_a_id);
            }
            if (searched_nodes.find(cur_edge.node_b_id) == searched_nodes.end()) {
                auto it_cur_map = map_edges.find(cur_edge.node_b_id);
                if (it_cur_map != map_edges.end()) {
                    for (auto& cur_set_edge : it_cur_map->second) {
                        v_edges.emplace_back(cur_set_edge);
                    }
                }
                searched_nodes.emplace(cur_edge.node_a_id);
            }
            std::cout << v_edges.size();
            v_edges.pop_back();
            
        }
        return false;
    }
    std::set<Edge>& build_shortest_edges_depth(std::priority_queue<Edge> edges, std::set<Edge>& added_edges) {
        //TODO: account for roads needed to connect previous nodes. This current code only adds the shortest edges it can find.
        std::set<OSMID> added_nodes;
        std::map<OSMID, std::set<Edge>> map_edges;
        added_edges.clear();
        while (!edges.empty()){
            auto cur_edge = edges.top();
            bool bAddEdge = false;
            //a little faster than searching the whole list, if node doesn't exist yet, add it.
            if (added_nodes.find(cur_edge.node_a_id) == added_nodes.end()) {
                bAddEdge = true;
            }
            else if (added_nodes.find(cur_edge.node_b_id) == added_nodes.end()) {
                bAddEdge = true;
            }
            /*auto it_cur_map = map_edges.find(cur_edge.node_a_id);
            if (it_cur_map != map_edges.end()) {
                if (it_cur_map->second.size() < 2) {
                    it_cur_map = map_edges.find(cur_edge.node_b_id);
                    if (it_cur_map != map_edges.end()) {
                        if (it_cur_map->second.size() < 2) {
                            bAddEdge = true;
                        }
                    }
                }
            }*/
            

            //if edge is needed to complete union
            //check if we can already get from node_a to node_b
            if (!bAddEdge) {
                if (!nodes_connected_depth(map_edges, cur_edge)) {
                    //the nodes should be connected but currenly aren't :(
                    //both nodes are already added.
                    bAddEdge = true;
                }
            }
            if (bAddEdge) {
                added_edges.emplace(cur_edge);
                added_nodes.emplace(cur_edge.node_a_id);
                added_nodes.emplace(cur_edge.node_b_id);
                map_edges[cur_edge.node_a_id].emplace(cur_edge);
                map_edges[cur_edge.node_b_id].emplace(cur_edge);
            }
            edges.pop();
        }
        return added_edges;
    }
    std::set<Edge>& build_shortest_edges(std::priority_queue<Edge> edges, std::set<Edge>& added_edges) {
        std::set<OSMID> added_nodes;
        std::map<OSMID, NodeUnion> m_node_union;
        NodeUnion node_union;
        node_union.make_set(nodes);
        added_edges.clear();
        for (const Edge& cur_edge = edges.top(); !edges.empty(); edges.pop()) {

            bool bAddEdge = false;
            //a little faster than searching the whole list, if node doesn't exist yet, add it.
            if (added_nodes.find(cur_edge.node_a_id) == added_nodes.end()) {
                bAddEdge = true;
            }
            else if (added_nodes.find(cur_edge.node_b_id) == added_nodes.end()) {
                bAddEdge = true;
            }
            if (!bAddEdge) {
                if (node_union.find(cur_edge.node_a_id) != node_union.find(cur_edge.node_b_id)) {
                    bAddEdge = true;
                }
            }


            if (bAddEdge) {
                added_edges.emplace(cur_edge);
                added_nodes.emplace(cur_edge.node_a_id);
                added_nodes.emplace(cur_edge.node_b_id);
                node_union.Union(cur_edge.node_a_id, cur_edge.node_b_id);
            }
        }
        return added_edges;
    }
    /*std::set<Edge> build_shortest_edges() {
        return build_shortest_edges(q_edges, min_edges);
    }*/
    void build_output(const std::string& file_name, std::set<Edge> min_edges) {
        auto total_roads = get_road_lengths(nodes, roads);
        double total_road_length = 0.0;
        for (auto& road : total_roads) {
            total_road_length+= road.second;
        }
        double total_weight = 0.0;
        int num_edges = 0;
        for (auto& edge : min_edges) {
            total_weight += edge.weight;
            ++num_edges;
        }
        std::ofstream ofile(file_name);
        if (ofile.is_open())
        {
            ofile << total_weight << std::endl;
            ofile << num_edges << std::endl;
            for (auto& edge : min_edges) {
                ofile << edge.node_a_id << " " << edge.node_b_id << std::endl;
            }
            ofile.close();
        }
        else {
            std::cout << "Could Not Write to " << file_name << std::endl;
        }
    }

    void build_output(const std::string& file_name) {
        return build_output(file_name, min_edges);
    }
    void build_output() {
        //first line : total length of edges built as float
        //second line : total # of edges built
        //two osm ids of nodes with space between
        return build_output("output.txt");
    }
    void fill() {
        load_nodes("nodes.txt",nodes);
        load_roads("roads.txt",roads);
        load_edges(nodes,roads,q_edges);
    }
    void run() {
        fill();
        build_shortest_edges(q_edges,min_edges);
        build_output("output.txt",min_edges);
    }
    void debug_all_nodes_exist() {
        for (auto& node : nodes) {
            const OSMID node_id = node.first;
            bool bFound = false;
            for (auto& edge : min_edges) {
                if (edge.node_a_id == node_id || edge.node_b_id == node_id) {
                    bFound = true;
                    break;
                }
            }
            if (!bFound) {
                std::cout << "Node id " << node_id << " is missing from min_edges" << std::endl;
            }
        }
    }
    std::map<OSMID, std::set<OSMID>> connected_nodes(std::priority_queue<Edge> edges) {
        std::map<OSMID, std::set<OSMID>> connect;
        while (!edges.empty()) {
            auto cur_edge = edges.top();
            connect[cur_edge.node_a_id].insert(cur_edge.node_b_id);
            connect[cur_edge.node_b_id].insert(cur_edge.node_a_id);
            edges.pop();
        }
        return connect;
    }
    void debug_connections() {
        auto orig_connect = connected_nodes(q_edges);
        std::priority_queue<Edge> new_edges;
        for (auto& edge : min_edges) {
            new_edges.emplace(edge);
        }
        auto new_connect = connected_nodes(new_edges);
        for (auto &orig_m : orig_connect) {
            auto &orig_set = orig_m.second;
            auto it_new_set = new_connect.find(orig_m.first);
            if (it_new_set != new_connect.end()) {
                std::set<OSMID> new_set = it_new_set->second;
                for (auto id : orig_set) {
                    if (new_set.find(id) == new_set.end()) {
                        //this will just print all removed edges
                        std::cout << "Connection Missing Between " << id << " and " << orig_m.first << std::endl;
                    }
                }
            }
        }

    }
    void debug() {
        fill();
        build_shortest_edges(q_edges, min_edges);
        debug_connections();
        //passed
        //debug_all_nodes_exist();
    }
};