// PP1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <iostream>
#include <chrono>
#include <cmath>

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

class CombinatorV2 {
public:
    CombinatorV2(size_t length, size_t count): blockCount(length), blockSize(std::ceil(std::log2(count))) {
        data = new long[(int)std::ceil(blockSize * blockCount / (sizeof(long) * 8))];
    };
private:
    long* data = nullptr;

    const size_t blockSize;
    const size_t blockCount;
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
        return {};
    }
}

std::optional<Graph> loadGraph(std::string path) {
    auto const rawData = loadData(path);
    if (!rawData.has_value()) return {};
    auto& data = rawData.value();
    try {
        Graph G = Graph::fromJSON(data["adjacency"]);
        return G;
    }
    catch (const std::exception&) {
        return {};
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

int getChromaticNumber(Graph& G, int chromNum = 1) {
    for (auto res = checkChromaticNumber(G, chromNum); !res.has_value() ? true : (/*print(res.value()),*/ false); res = checkChromaticNumber(G, ++chromNum));
    return chromNum;
}

int main(int argc, char* argv[]) {

    if (argc < 2) { return 1; }

    

    if (auto G = loadGraph(argv[1])) {
        auto start = std::chrono::high_resolution_clock::now();
        int chromNum = getChromaticNumber(G.value());
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << chromNum << " calculated in: " << duration.count() << std::endl << "used method: 1";
    }
    else {
        std::cout << -1;
    }

    

}
