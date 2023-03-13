// PP1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <iostream>
#include <chrono>

#include <fstream>
#include <nlohmann/json.hpp>

#include "../combinatorV2/combinator.h";


using json = nlohmann::json;

class Graph {
public:
    Graph(int nodes) {
        this->nodes = nodes;

        int size = nodes * nodes;

        this->data.reserve(size);
        this->data.resize(size);
    }

    void setAdjacency(int node1, int node2, bool adjacency) {
        this->data.at(node1 * this->nodes + node2) = adjacency;
    }

    bool areAdjacent(int node1, int node2) {
        return this->data.at(node1 * this->nodes + node2);
    }

    int size() {
        return this->nodes;
    }

    static Graph fromJSON(json data) {
        int nodes = data.size();
        Graph G = Graph(nodes);

        for (int i = 0; i < nodes; i++) {
            auto& row = data[i];
            for (int j = 0; j < nodes; j++) {
                G.setAdjacency(j, i, row[j]);
            }
        }

        return G;
    }
private:
    int nodes = 0;

    std::vector<bool> data = {};
};

Graph loadGraph(std::string path) {
    auto data = json::parse(std::ifstream(path));
    Graph G = Graph::fromJSON(data["adjacency"]);
    return G;
}

bool checkChromaticNumber(Graph& G, int num) {
    const int nodes = G.size();

    auto c = CombinatorV2(nodes, num);
    do {
        for (int i = 0; i < c.size(); i++) {
            auto group = c.at(i);
            for (int j = 0; j < nodes; j++) {
                if (j == i) continue;
                if (G.areAdjacent(i, j) && group == c.at(j)) goto outer;
            }
        }
        return true;
        outer: continue;
    } while (!c.next());

    return false;
}

int getChromaticNumber(Graph& G, int chromNum = 1) {
    while (!checkChromaticNumber(G, chromNum)) {
        chromNum++;
    }
    return chromNum;
}

int main(int argc, char* argv[]) {
    
    if (argc < 2) { return 1; }
    
    auto G = loadGraph(argv[1]);

    auto start = std::chrono::high_resolution_clock::now();
    int chromNum = getChromaticNumber(G);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << chromNum << " calculated in: " << duration.count() << std::endl << "used method: 1";

}
