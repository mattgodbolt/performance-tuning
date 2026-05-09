#include <cstdio>

#include "bench.hpp"

void newOrder(char *buf, const char *id, int price, int quantity) {
  sprintf(buf, "NEW %s %d %d", id, price, quantity);
}

int main(int, const char **) {
  char buf[2048];
  newOrder(buf, "TWTR", 0, 0);
  bench::measure(100'000'000, [&](long long i) {
    newOrder(buf, "TWTR", static_cast<int>(i & 0xff),
             static_cast<int>((i & 0xfff) + 1));
    bench::do_not_optimize(buf);
  });
  return 0;
}
