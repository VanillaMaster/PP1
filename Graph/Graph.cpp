#include "Graph.h"

Graph::Graph(int nodes) {
    this->nodes = nodes;

    int size = nodes * nodes;

    this->data.reserve(size);
    this->data.resize(size);
}

void Graph::setAdjacency(int node1, int node2, bool adjacency) {
    this->data.at(node1 * this->nodes + node2) = adjacency;
}

bool Graph::areAdjacent(int node1, int node2) {
    return this->data.at(node1 * this->nodes + node2);
}

int Graph::size() {
    return this->nodes;
}

Graph Graph::fromJSON(json data) {
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

Graph loadGraph(std::string path) {
    auto data = json::parse(std::ifstream(path));
    Graph G = Graph::fromJSON(data["adjacency"]);
    return G;
}