#include <iostream>
#include <sstream>
#include <string>

#include "bench.hpp"

using namespace std;

string newOrder(string id, int price, int quantity) {
  stringstream s;
  s << "NEW " << id << " " << price << " " << quantity;
  return s.str();
}

int main(int, const char **) {
  newOrder("TWTR", 0, 0); // warm up locale init etc.
  bench::measure(10'000'000, [](long long i) {
    auto s = newOrder("TWTR", static_cast<int>(i & 0xff),
                      static_cast<int>((i & 0xfff) + 1));
    bench::do_not_optimize(s);
  });
  return 0;
}
