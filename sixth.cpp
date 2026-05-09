// std::format (allocating) version - parallel to first/second/third etc.
#include <cstdint>
#include <cstring>
#include <ctime>
#include <format>
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

string newOrder(string_view stock, int price, int quantity) {
  return format("NEW {} {} {}", stock, price, quantity);
}

int main(int, const char **) {
  constexpr auto numIters = 100000000;
  // Sanity check
  if (newOrder("TWTR", 0, 1) != "NEW TWTR 0 1") {
    cout << "fail" << endl;
    return 1;
  }
  if (newOrder("TWTR", -98760, 102030) != "NEW TWTR -98760 102030") {
    cout << "fail" << endl;
    return 1;
  }
  auto begin = timestamp();
  for (int i = 0; i < numIters; ++i) {
    auto s = newOrder("TWTR", i & 0xff, (i & 0xfff) + 1);
    asm volatile("" : : "r,m"(s) : "memory");
  }
  auto nsTaken = timestamp() - begin;
  cout << numIters << " orders in " << nsTaken << "ns" << endl;
  cout << (double)nsTaken / numIters << "ns / order" << endl;
  return 0;
}
