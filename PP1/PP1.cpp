// PP1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <iostream>

#include <fstream>
#include <nlohmann/json.hpp>

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

class SimpleCombinator {
public:
    SimpleCombinator(int length, int amount) {
        this->sequence.reserve(length);
        this->sequence.resize(length);
        this->sequence.at(0) = -1;

        this->amount = amount;
    }

    bool hasNext(){
        return !this->done;
    }
    
    void next() {
        if (this->done) return;
        bool done = true;
        bool inc = true;
        for (int i = 0; i < this->sequence.size(); i++)
        {
            if (inc) {
                this->sequence.at(i)++;
                inc = this->sequence.at(i) >= this->amount;
                if (inc) {
                    this->sequence.at(i) = 0;
                }
            }
            if (done) done = this->sequence.at(i) + 1 >= this->amount;
        }
        this->done = done;
    }

    std::vector<int> sequence = {};

private:
    int amount = 0;
    bool done = false;
};

std::optional<json> loadData(std::string path) {
    try
    {
        std::ifstream f(path);
        auto result = json::parse(f);
        return result;
    }
    catch (const std::exception&)
    {
        return NULL;
    }
}

std::optional<std::vector<int>> checkChromaticNumber(Graph& G, int num) {
    const int nodes = G.size();
    auto c = SimpleCombinator(nodes, num);

    while (c.hasNext()) {
        c.next();
        for (int i = 0; i < nodes; i++) {
            auto group = c.sequence.at(i);
            for (int j = 0; j < nodes; j++) {
                if (j == i) continue;
                if (G.areAdjacent(i , j) && group == c.sequence.at(j)) goto outer;
            }
        }
        return std::vector<int>(c.sequence);
        outer: continue;
    }
    return {};
}

void print(std::vector<int>& data) {
    for (auto num : data) {
        std::cout << num << " ";
    }
    std::cout << "\n";
}

int main(int argc, char* argv[]) {

    if (argc < 2) { return 1; }

    auto resultOpt = loadData(argv[1]);

    if (!resultOpt.has_value()) { return 2; }

    auto& data = resultOpt.value();

    Graph G = Graph::fromJSON(data["adjacency"]);

    int chromNum = 1;

    for (auto res = checkChromaticNumber(G, chromNum); !res.has_value() ? true : (print(res.value()), false); res = checkChromaticNumber(G, ++chromNum));
    
    std::cout << chromNum;

}
