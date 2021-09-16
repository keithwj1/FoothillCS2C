// homework9.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <cmath>
#include <cstdint>
#include <exception>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <vector>
#include <string>
#include "classes.h"




int main(int argc, char* argv[]) {
    //auto debugger = RoadFinderDebug();
    //debugger.test_all();
    auto road_finder = RoadFinder();
    //road_finder.debug();
    road_finder.run();
    return 0;
}



int main_old(int argc, char* argv[]) {
	//auto debugger = RoadFinderDebug();
	//debugger.test_all();
	auto road_finder = RoadFinder();
    const double min_road_len = 5.0; // km
    std::map<OSMID, Node> nodes = road_finder.load_nodes(argc > 1 ? argv[1] : "nodes.txt");
    std::map<OSMID, Road> roads = road_finder.load_roads(argc > 2 ? argv[2] : "roads.txt");
    std::map<std::string, double> road_lengths = road_finder.get_road_lengths(nodes, roads);

    std::cout << "Roads that are at least " << min_road_len << "km:\n";
    for (const auto& name_length : road_lengths) {
        if (name_length.second > min_road_len) {
            std::cout << "  " << name_length.first << ": " << name_length.second << "km\n";
        }
    }
    std::cout << std::endl;
    road_finder.load_edges();
    //road_finder.min_edges = road_finder.build_shortest_edges();
    road_finder.build_output();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
