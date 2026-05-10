// std::format (allocating) version - parallel to first/second/third etc.
#include <cstring>
#include <format>
#include <iostream>
#include <string>
#include <string_view>

#include "bench.hpp"

using namespace std;

string newOrder(string_view stock, int price, int quantity) {
  return format("NEW {} {} {}", stock, price, quantity);
}

int main(int, const char **) {
  if (newOrder("TWTR", 0, 1) != "NEW TWTR 0 1") {
    cout << "fail" << endl;
    return 1;
  }
  if (newOrder("TWTR", -98760, 102030) != "NEW TWTR -98760 102030") {
    cout << "fail" << endl;
    return 1;
  }
  bench::measure(10'000'000, [](long long i) {
    auto s = newOrder("TWTR", static_cast<int>(i & 0xff),
                      static_cast<int>((i & 0xfff) + 1));
    bench::do_not_optimize(s);
  });
  return 0;
}
