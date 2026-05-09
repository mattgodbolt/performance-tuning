// fmt::format_to into a fixed buffer (parallel to seventh.cpp's std::format_to).
#include <cstdint>
#include <cstring>
#include <ctime>
#include <fmt/core.h>
#include <iostream>

using namespace std;

typedef uint64_t tstamp_t;

static inline tstamp_t timestamp() {
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC, &tp);
  uint64_t nanos = 1000000000;
  nanos *= tp.tv_sec;
  return nanos + tp.tv_nsec;
}

void newOrder(char *buf, const char *stock, int price, int quantity) {
  auto out = fmt::format_to(buf, "NEW {} {} {}", stock, price, quantity);
  *out = '\0';
}

int main(int, const char **) {
  constexpr auto numIters = 100000000;
  char buf[2048];
  newOrder(buf, "TWTR", 0, 1);
  if (strcmp(buf, "NEW TWTR 0 1")) {
    cout << buf << endl;
    return 1;
  }
  newOrder(buf, "TWTR", -98760, 102030);
  if (strcmp(buf, "NEW TWTR -98760 102030")) {
    cout << buf << endl;
    return 1;
  }
  auto begin = timestamp();
  for (int i = 0; i < numIters; ++i) {
    newOrder(buf, "TWTR", i & 0xff, (i & 0xfff) + 1);
    asm volatile("" : : "r,m"(buf) : "memory");
  }
  auto nsTaken = timestamp() - begin;
  cout << numIters << " orders in " << nsTaken << "ns" << endl;
  cout << (double)nsTaken / numIters << "ns / order" << endl;
  return 0;
}
