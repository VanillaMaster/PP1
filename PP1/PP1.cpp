// PP1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <iostream>
#include <chrono>


#include "../combinatorV2/combinator.h";
#include "../Graph/Graph.h"

//constexpr int BiL = sizeof(long) * 8;

bool checkChromaticNumber(Graph& G, int num) {
    std::cout << num << std::endl;
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
    
    auto c = CombinatorV3(8, 2);
    c.useRange(3, 0);
    do {
        std::cout << c.toString() << std::endl;
    } while (!c.next());
    return 0;

    if (argc < 2) { return 1; }
    
    auto G = loadGraph(argv[1]);

    auto start = std::chrono::high_resolution_clock::now();
    int chromNum = getChromaticNumber(G, 3);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << chromNum << " calculated in: " << duration.count() << std::endl << "used method: 1";

}
