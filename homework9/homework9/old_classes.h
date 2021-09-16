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
#include <assert.h>
#include <queue>
#include <set>



/*
TODO:
Build all nodes and road.
Decide if two nodes are "connected"
Create separate trees if two nodes are not connected
Create new graph without any edges.
Add minimum edges
use link implementation because it is easier
*/


class MyException : public std::exception {
    const char* msg;
public:
    MyException(const char* msg) : msg(msg) {}

    const char* what() const noexcept {
        return msg;
    }
};

typedef int_least64_t OSMID;



class Node {
private:
public:
    OSMID id;
    double x, y;
    std::map<Node, double> edges;
    Node() : id(0), x(0.0), y(0.0) {}
    Node(OSMID id, double x, double y) : id(id), x(x), y(y) {}
    bool operator==(const Node& rhs) const noexcept {
        return (id == rhs.id);
    }
    bool operator<(const Node& rhs) const noexcept{
        return (id < rhs.id);
    }
    bool operator>(const Node& rhs) const noexcept {
        return (id > rhs.id);
    }
    void add_shortest_edge(OSMID road_id,Node &x, Node &y, double weight) {
        Node node = x;
        if (node == *this) { node = y; }
        auto it_edge = edges.find(node);
        if (it_edge == edges.end()) {
            edges[node] = weight;
        }
        else {
            //edge already exists, keep shortest one
            if (it_edge->second > weight) {
                edges[node] = weight;
            }
        }
    }
};

class Edge {
public:
    OSMID road;
    Node a, b;
    double weight;
    Edge(OSMID road, Node& x, Node& y, double weight) : road(road), a(x), b(y), weight(weight) {
        //always make the smallest node given node a so a
        if (x.id > y.id) {
            a = y;
            b = x;
        }
    }
    bool operator<(const Edge& rhs) const noexcept {
        return (weight < rhs.weight);
    }
    bool operator>(const Edge& rhs) const noexcept {
        return !(*this < rhs);
    }
};
class NodePair {
private:
public:
    Node a, b;
    OSMID road_id = NULL;
    bool operator==(const NodePair& rhs) const {
        return ((a == rhs.a && b == rhs.b) || (b == rhs.a && a == rhs.b));
    }
    NodePair(Node a, Node b,OSMID road_id) : a(a), b(b), road_id(road_id) {}
    bool operator<(const NodePair& rhs) const noexcept {
        return (a.id < rhs.a.id);
    }
    bool operator>(const NodePair& rhs) const noexcept {
        return (a.id > rhs.a.id);
    }
};
class Road {
public:
    OSMID id;
    std::string name;
    std::vector<OSMID> path;

    Road() : id(0), name("") {}
    Road(OSMID id, std::string name) : id(id), name(name) {}
    Road(OSMID id, std::string name, std::vector<OSMID> path) : id(id), name(name), path(path) {}
    bool operator<(const Node& rhs) const noexcept {
        return (id < rhs.id);
    }
};



class RoadFinder {
private:

public:
    std::map<OSMID, Node> nodes;
    std::map<OSMID, Road> roads;
    //road id, Edge

    //std::vector<Edge> edges;
    std::map<NodePair, double> edges;
    std::priority_queue<Edge> q_edges;
    std::set<Edge> min_edges;
    //std::multimap<OSMID,Edge> edges;
    std::map<std::string, double> road_lengths;
    template <typename T>
    const unsigned int hash(const T& value) const {
        return std::hash<T>{}(value);
    }
    Node& get_node(std::map<OSMID, Node> &nodes, const OSMID id) {
        return nodes[id];
    }
    Node& get_node(const OSMID id) {
        return get_node(nodes, id);
    }
    

    std::map<OSMID, Node> load_nodes(const std::string& file_name) {
        nodes = get_nodes_map(file_name);
        return nodes;
    }
    std::map<OSMID, Road> load_roads(const std::string& file_name) {
        roads = get_roads_map(file_name);
        return roads;
    }
    std::map<OSMID, Node> get_nodes_map(const std::string& file_name) {
        std::ifstream nodes_file(file_name);
        std::map<OSMID, Node> nodes;
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
                nodes[osmid] = { osmid,x, y };
            }
        }
        if (nodes_file.bad()) {
            throw MyException("Error reading nodes from file.");
        }
        nodes_file.close();
        return nodes;
    }
    std::map<OSMID, Road> get_roads_map(const std::string& file_name) {
        std::map<OSMID, Road> roads;
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
                getline(roads_file, r.name);
                r.id = osmid;
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
        return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }
    double distance(const OSMID x, const OSMID y) {
        Node a = get_node(x);
        Node b = get_node(y);
        return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
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
    std::map<std::string, double> get_road_lengths() {
        road_lengths = get_road_lengths(nodes, roads);
        return road_lengths;
    }
    void add_edges_to_nodes() {
        for (auto& road : roads) {
            auto path = road.second.path;
            while (!path.empty()) {
                auto cur_node_id = path.back();
                auto& cur_node = get_node(cur_node_id);
                path.pop_back();
                for (auto& node_id : path) {
                    auto& second_node = get_node(node_id);
                    cur_node.add_shortest_edge(road.first, cur_node, second_node, distance(cur_node, second_node));
                }
            }
        }
    }
    std::priority_queue<Edge> & load_edges(const std::map<OSMID, Node>& nodes, const std::map<OSMID, Road>& roads, std::priority_queue<Edge> &edges) {
        for (auto& road : roads) {
            auto path = road.second.path;
            while (!path.empty()) {
                auto cur_node_id = path.back();
                auto& cur_node = get_node(cur_node_id);
                path.pop_back();
                for (auto& node_id : path) {
                    auto& second_node = get_node(node_id);
                    Edge e(road.first, cur_node, second_node, distance(cur_node,second_node));
                    edges.push(e);
                }
            }
        }
        return edges;
    }
    std::priority_queue<Edge>& load_edges() {
        return load_edges(nodes, roads, q_edges);
    }
    std::map<NodePair, double>& load_edges_old(const std::map<OSMID, Node>& nodes, const std::map<OSMID, Road>& roads, std::map<NodePair, double>& edges) {
        for (auto& road : roads) {
            auto path = road.second.path;
            while (!path.empty()) {
                auto cur_node_id = path.back();
                auto& cur_node = get_node(cur_node_id);
                path.pop_back();
                for (auto& node_id : path) {
                    auto& second_node = get_node(node_id);
                    NodePair pair(cur_node, second_node, road.first);
                    auto weight = distance(cur_node, second_node);
                    auto it_edge = edges.find(pair);

                    if (it_edge == edges.end()) {
                        edges[pair] = weight;
                    }
                    else {
                        //edge already exists, keep shortest one
                        if (it_edge->second > weight) {
                            edges[pair] = weight;
                        }
                    }
                }
            }
        }
        return edges;
    }
    std::map<NodePair,double> &load_edges_old() {
        return load_edges_old(nodes, roads, edges);
    }
    template <typename T>
    bool exists_in_vector(std::vector<T> &input, T &object) {
        return (std::find(input.begin(), input.end(), object) == input.end());
    }
    std::set<Edge> & build_shortest_edges(const std::map<OSMID, Node>& nodes, std::priority_queue<Edge>& edges, std::set<Edge> &added_edges) {
        std::set<OSMID> added_nodes;
        //std::vector<Node> added_nodes;
        while (!edges.empty()) {
            auto cur_edge = edges.top();
            bool bAddEdge = false;
            if (added_nodes.find(cur_edge.a.id) == added_nodes.end()){
                bAddEdge = true;
                added_nodes.emplace(cur_edge.a.id);
            }
            if (added_nodes.find(cur_edge.b.id) == added_nodes.end()) {
                bAddEdge = true;
                added_nodes.emplace(cur_edge.b.id);
            }
            if (bAddEdge) {
                //one node is not yet in the list, add nodes
                added_edges.emplace(cur_edge);
            }
            edges.pop();
        }
        return added_edges;
    }
    std::set<Edge> build_shortest_edges() {
        return build_shortest_edges(nodes, q_edges, min_edges);
    }
    void build_output(const std::string& file_name, std::set<Edge> edges) {
        double total_weight = 0.0;
        int num_edges = 0;
        for (auto& edge : edges) {
            total_weight += edge.weight;
            ++num_edges;
        }
        std::ofstream ofile(file_name);
        if (ofile.is_open())
        {
            ofile << total_weight << std::endl;
            ofile << num_edges << std::endl;
            for (auto& edge : edges) {
                ofile << edge.a.id << " " << edge.b.id << std::endl;
            }
            ofile.close();
        }
    }
    void build_output() {
        //first line : total length of edges built as float
        //second line : total # of edges built
        //two osm ids of nodes with space between
        return build_output("output.txt", min_edges);
    }
    void fill() {
        load_nodes("nodes.txt");
        load_roads("roads.txt");
        load_edges();
        
    }
};

class RoadFinderDebug {
public:
    int test_fill_print() {
        const double min_road_len = 5.0; // km
        auto road_finder = RoadFinder();
        auto nodes = road_finder.load_nodes("nodes.txt"); //nodes
        auto roads = road_finder.load_roads("roads.txt");//edges
        std::map<std::string, double> road_lengths = road_finder.get_road_lengths(nodes, roads);

        std::cout << "Roads that are at least " << min_road_len << "km:\n";
        for (const auto& name_length : road_lengths) {
            if (name_length.second > min_road_len) {
                std::cout << "  " << name_length.first << ": " << name_length.second << "km\n";
            }
        }
        std::cout << std::endl;
        return 0;
    }
    int test_edges() {
        auto road_finder = RoadFinder();
        road_finder.fill();
        /*for (auto& edge : road_finder.edges) {
            std::cout << "Road ID = " << edge.road << " Between Nodes " << edge.x << " and " << edge.y << " with weight " << edge.weight << std::endl;
        }*/
        return 0;
    }
    int test_make_graph() {
        auto road_finder = RoadFinder();
        auto nodes = road_finder.load_nodes("nodes.txt"); //nodes
        auto roads = road_finder.load_roads("roads.txt");//edges
        
        return 0;
    }
    int test_fill() {
        RoadFinder road_finder = RoadFinder();
        auto nodes = road_finder.load_nodes("nodes.txt");
        auto roads = road_finder.load_roads("roads.txt");
        std::map<std::string, double> road_lengths = road_finder.get_road_lengths();
        for (const auto& name_length : road_lengths) {
            if (name_length.first == "Adobe Wells"){
                assert(name_length.second == double(6.31737));
            }
            else if (name_length.first == "Page Mill Road") {
                assert(name_length.second == double(11.4679));
            }
            else if (name_length.first == "private residential road/driveway") {
                assert(name_length.second == double(0.0626805));
            }
            else if (name_length.first == "(A to D) to A") {
                assert(name_length.second == double(0.0424504));
            }
        }
        return 0;
    }
    int test_all() {
        //test_fill_print();
        //test_make_graph();
        test_edges();
        return 0;
    }
};