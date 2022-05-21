#include <cstdint>
#include <ctime>
#include <iostream>
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

void newOrder(char *buf, const char *id, int price, int quantity) {
    sprintf(buf, "NEW %s %d %d", id, price, quantity);
}

int main(int, const char **) {
    constexpr auto numIters = 100000000;
    char buf[2048];
    newOrder(buf, "TWTR",0,0);
    auto begin = timestamp();
    for (int i = 0; i < numIters; ++i) {
        newOrder(buf, "TWTR", i & 0xff, (i & 0xfff) + 1);
    }
    auto nsTaken = timestamp() - begin;
    cout << numIters << " orders in " << nsTaken << "ns" << endl;
    cout << (double)nsTaken / numIters << "ns / order" << endl;
    return 0;
}
