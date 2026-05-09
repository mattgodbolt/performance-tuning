#include <cstdint>
#include <cstring>
#include <iostream>

#include "bench.hpp"

using namespace std;

static unsigned const PowersOf10[] = {1,         10,        100,     1000,
                                      10000,     100000,    1000000, 10000000,
                                      100000000, 1000000000}; // etc

static unsigned numDigits(unsigned v) {
  auto log2 = 31 - __builtin_clz(v);
  auto log10Approx = (log2 + 1) * 1233 >> 12;
  auto log10 = log10Approx - (v < PowersOf10[log10Approx]);
  return log10 + 1;
}

class Format {
  char _buffer[2048];
  static uint16_t _lookup[100];
  int _ptr;

public:
  static void init() {
    for (int i = 0; i < 100; ++i) {
      auto dig1 = '0' + (i % 10);
      auto dig2 = '0' + ((i / 10) % 10);
      _lookup[i] = dig2 | dig1 << 8;
    }
  }
  Format() : _ptr(0) {}
  inline void reset() { _ptr = 0; }
  const char *c_str() const { return _buffer; }
  void append(char c) { _buffer[_ptr++] = c; }
  void finish() { append('\x00'); }

  void append(const char *data) {
    while (*data)
      append(*data++);
  }

  template <int N>
  inline void decimalPadAppend2(unsigned value) {
    if (N >= 2) {
      for (int i = N - 2; i >= 0; i -= 2) {
        auto twoDigits = value % 100;
        value /= 100;
        memcpy(_buffer + _ptr + i, &_lookup[twoDigits], sizeof(uint16_t));
      }
    }
    if (N & 1)
      _buffer[_ptr] = value % 10 + '0';
    _ptr += N;
  }

  void decimalAppendNonZero(unsigned value) {
    auto digits = numDigits(value);
#define CASE(XX)                                                               \
  case XX:                                                                     \
    decimalPadAppend2<XX>(value);                                              \
    break
    switch (digits) {
      CASE(1);
      CASE(2);
      CASE(3);
      CASE(4);
      CASE(5);
      CASE(6);
      CASE(7);
      CASE(8);
      CASE(9);
      CASE(10);
    default:
      __builtin_unreachable();
      break;
    }
  }

  void decimalAppend(int value) {
    if (value == 0) {
      append('0');
      return;
    } else if (value < 0) {
      append('-');
      value = -value;
    }
    decimalAppendNonZero(value);
  }
};

uint16_t Format::_lookup[100];

void newOrder(Format &format, const char *stock, int price, int quantity) {
  format.append("NEW ");
  format.append(stock);
  format.append(' ');
  format.decimalAppend(price);
  format.append(' ');
  format.decimalAppendNonZero(quantity);
  format.finish();
}

int main(int, const char **) {
  Format::init();
  Format buf;
  newOrder(buf, "TWTR", 0, 1);
  if (strcmp(buf.c_str(), "NEW TWTR 0 1")) {
    cout << buf.c_str() << endl;
    return 1;
  }
  buf.reset();
  newOrder(buf, "TWTR", -98760, 102030);
  if (strcmp(buf.c_str(), "NEW TWTR -98760 102030")) {
    cout << buf.c_str() << endl;
    return 1;
  }
  bench::measure(1'000'000'000LL, [&](long long i) {
    buf.reset();
    newOrder(buf, "TWTR", static_cast<int>(i & 0xff),
             static_cast<int>((i & 0xfff) + 1));
    bench::do_not_optimize(buf);
  });
  return 0;
}
