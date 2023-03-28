

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <format>

#include "../combinatorV2/combinator.h";
#include "../Graph/Graph.h"

constexpr int ThredNumber = 4;

struct context {
    bool done;
    bool ok;
    int num;
    int slice;
};

void threadFunction(context* ctx, Graph* G) {

    const int nodes = G->size();
    auto c = CombinatorV3(nodes, ctx->num, std::to_string(ctx->num) + "::" + std::to_string(ctx->slice) + ": ");
    c.useRange(ThredNumber, ctx->slice);

    do {
        for (int i = 0; i < c.size(); i++) {
            auto group = c.at(i);
            for (int j = 0; j < nodes; j++) {
                if (j == i) continue;
                if (G->areAdjacent(i, j) && group == c.at(j)) goto outer;
            }
        }
        ctx->ok = true;
        ctx->done = true;
        return;
    outer: continue;
    } while (!c.next());

    ctx->ok = false;
    ctx->done = true;
    return;

    // do smth
}

int main(int argc, char* argv[]) {

    if (argc < 2) { return 1; }

    auto G = loadGraph(argv[1]);


    auto start = std::chrono::high_resolution_clock::now();


    context ctxs[ThredNumber];
    for (size_t i = 0; i < ThredNumber; i++) { ctxs[i] = { true, false, 0, 0 }; };

    bool keepSummon = true;
    bool keepWait = true;

    int chromeNumberToCheck = 1;
    int slice = -1;

    while (keepWait) {
        keepWait = true;
        for (size_t i = 0; i < ThredNumber; i++) {
            if (ctxs[i].done) {
                if (ctxs[i].num != 0) std::cout << std::to_string(ctxs[i].num) + "::" + std::to_string(ctxs[i].slice) + ": died\n";

                if (ctxs[i].ok && keepSummon) {
                    keepSummon = false;
                    std::cout << "first result getted (stop summon)\n" + std::to_string(ctxs[i].num);
                }

                if (keepSummon) {
                    slice++;
                    if (slice >= ThredNumber) {
                        chromeNumberToCheck++;
                        slice = 0;
                    }
                    ctxs[i] = {false, false, chromeNumberToCheck, slice };
                    std::cout << std::to_string(chromeNumberToCheck) + "::" + std::to_string(slice) + ": summoned\n";
                    std::thread t(threadFunction, &(ctxs[i]), &G);
                    t.detach();
                }
            }
            keepWait = keepWait && ctxs[i].done;
        }
        keepWait = !keepWait;
    }

    int chromeNumber = INT_MAX;

    for (size_t i = 0; i < ThredNumber; i++) {
        if (ctxs[i].ok) {
            chromeNumber = std::min(ctxs[i].num, chromeNumber);
        }
    };


    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << chromeNumber << " calculated in: " << duration.count() << std::endl << "used method: 2";
}
