// fmt::format_to into a fixed buffer (parallel to seventh.cpp's
// std::format_to).
#include <cstring>
#include <fmt/core.h>
#include <iostream>

#include "bench.hpp"

using namespace std;

void newOrder(char *buf, const char *stock, int price, int quantity) {
  auto out = fmt::format_to(buf, "NEW {} {} {}", stock, price, quantity);
  *out = '\0';
}

int main(int, const char **) {
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
  bench::measure(10'000'000, [&](long long i) {
    newOrder(buf, "TWTR", static_cast<int>(i & 0xff),
             static_cast<int>((i & 0xfff) + 1));
    bench::do_not_optimize(buf);
  });
  return 0;
}
