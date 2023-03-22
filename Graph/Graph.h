#pragma once
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

class Graph {
public:
    Graph(int nodes);

    void setAdjacency(int node1, int node2, bool adjacency);

    bool areAdjacent(int node1, int node2);

    int size();

    static Graph fromJSON(json data);
private:
    int nodes = 0;
    std::vector<bool> data = {};
};

Graph loadGraph(std::string path);