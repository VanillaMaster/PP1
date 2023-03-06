// PP1.5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <set>
#include <iostream>
#include <chrono>

#include <fstream>
#include <nlohmann/json.hpp>

#include <intrin.h>

const int SoL = sizeof(long) * 8;

using json = nlohmann::json;

class Graph {
public:
    Graph(int size) {
        this->chunkSize = (size / (sizeof(long) * 8) ) + 1;
        this->nodes = size;
        data.reserve(chunkSize * size);
        data.resize(chunkSize * size);
        std::fill(data.begin(), data.end(), ~0L);
    }

    void merge(int node1, int node2) {
        for (int i = 0; i < this->chunkSize; i++) {
            this->data.at((node1 * chunkSize) + i) &= this->data.at((node2 * chunkSize) + i);
        }
    }

    static Graph fromJSON(json data) {
        //int const SoL = sizeof(long) * 8;

        int nodes = data.size();
        Graph G = Graph(nodes);

        for (int i = 0; i < nodes; i++) {
            auto& row = data[i];
            int cShift = i * G.chunkSize;

            int j = 0;
            bool hasWork = true;
            while (hasWork) {
                unsigned long& cElem = G.data.at(cShift + (j / SoL));
                int indxe = j % SoL;
                if (row[j]) {
                    cElem &= ~(1UL << indxe);
                }
                else {
                    cElem |= 1UL << indxe;
                }
                hasWork = (++j < nodes);
                if (!hasWork) {
                    cElem &= ~(0UL) >> (SoL - indxe - 1);
                }
            }
        }

        return G;
    }

    static int getChromaticNumber(Graph& G) {

        int chromeNum = 0;
        std::set<int> colored = {};

        for (int i = 0; i < G.nodes; i++) {
            if (colored.contains(i)) continue;
            chromeNum++;
            colored.insert(i);
            if (colored.size() >= G.nodes) break;
            for (int j = 0; j < G.chunkSize; j++) {
                unsigned long& data = G.data.at((i * G.chunkSize) + j);
                const int index = j * SoL;
                unsigned long unadjusted = 0;
                while (_BitScanForward(&unadjusted, data)) {
                    const int unadjustedIndex = index + unadjusted;
                    G.merge(i, unadjustedIndex);
                    colored.insert(unadjustedIndex);
                }
            }
        }

        return chromeNum;
    }

private:
    int chunkSize = 0;
    int nodes = 0;

    std::vector<unsigned long> data = {};
};

int main(int argc, char* argv[])
{
    if (argc < 2) { return 1; }

    auto data = json::parse(std::ifstream(argv[1]));
    Graph G = Graph::fromJSON(data["adjacency"]);

    auto start = std::chrono::high_resolution_clock::now();
    int chromNum = Graph::getChromaticNumber(G);
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    
    std::cout << chromNum << " calculated in: " << duration.count() << std::endl << "used method: 1.5";
}
