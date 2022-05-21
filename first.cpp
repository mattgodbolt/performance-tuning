#include <cstdint>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

typedef uint64_t tstamp_t;

static inline tstamp_t timestamp() {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    uint64_t nanos = 1000000000;
    nanos *= tp.tv_sec;
    return nanos + tp.tv_nsec;
}

string newOrder(string id, int price, int quantity) {
    stringstream s;
    s << "NEW " << id << " " << price << " " << quantity;
    return s.str();
}

int main(int, const char **) {
    constexpr auto numIters = 100000000;
    newOrder("TWTR",0,0);
    auto begin = timestamp();
    for (int i = 0; i < numIters; ++i) {
        newOrder("TWTR", i & 0xff, (i & 0xfff) + 1);
    }
    auto nsTaken = timestamp() - begin;
    cout << numIters << " orders in " << nsTaken << "ns" << endl;
    cout << (double)nsTaken / numIters << "ns / order" << endl;
    return 0;
}
