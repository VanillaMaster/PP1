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

void colored_mask_insert(std::vector<unsigned long>& self, int index) {
    unsigned long& data = self.at(index / SoL);
    data &= ~(1UL << (index % SoL));
}

bool colored_mask_has(std::vector<unsigned long>& self, int index) {
    unsigned long& data = self.at(index / SoL);
    return (data & (1UL << index % SoL)) == 0;
}

int colored_mask_size(std::vector<unsigned long>& self) {
    int i = 0;
    unsigned long* data = &(self.at(i));
    unsigned long index = 0;

    while (!_BitScanForward(&index, *data)) {
        data = &(self.at(++i));
    }

    return (SoL * i) + (int)index;

}


class Graph {
public:
    Graph(int size) {
        this->chunkSize = (size / (sizeof(long) * 8) ) + 1;
        this->nodes = size;
        //data.reserve(chunkSize * size);
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

        std::vector<unsigned long> colored_mask(G.chunkSize);
        std::fill(colored_mask.begin(), colored_mask.end(), ~0UL);

        for (int i = 0; i < G.nodes; i = colored_mask_size(colored_mask)) {
            //if (colored_mask_has(colored_mask, i)) continue;

            chromeNum++;
            colored_mask_insert(colored_mask, i);

            if (colored_mask_size(colored_mask) >= G.nodes) {
                break;
            }
            for (int j = 0; j < G.chunkSize; j++) {
                unsigned long& data = G.data.at((i * G.chunkSize) + j);
                data &= colored_mask.at(j);
                const int index = j * SoL;
                unsigned long unadjusted = 0;
                while (_BitScanForward(&unadjusted, data)) {
                    const int unadjustedIndex = index + unadjusted;
                    G.merge(i, unadjustedIndex);
                    colored_mask_insert(colored_mask, unadjustedIndex);
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
