#include <cstdint>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

typedef uint64_t tstamp_t;

class Format {
    char _buffer[2048];
    int _ptr;
public:
    Format() : _ptr(0) {}
    inline void reset() { _ptr = 0; }
    const char* c_str() const { return _buffer; }
    void append(char c) { _buffer[_ptr++] = c; }
    void finish() { append('\x00'); }

    void append(const char *data) {
        while (*data) append(*data++);
    }

    void decimalAppendNonNeg(unsigned value) {
        int startPos = _ptr;
        do {
            append(static_cast<char>(value % 10) + '0');
            value /= 10;
        } while (value);
        // Reverse the digits.
        auto end = &_buffer[_ptr - 1];
        auto start = &_buffer[startPos];
        while (end > start)
            std::swap(*start++, *end--);
    }

    void decimalAppend(int value) {
        if (value < 0) {
            append('-');
            value = -value;
        }
        decimalAppendNonNeg(value);
    }
};

static inline tstamp_t timestamp() {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    uint64_t nanos = 1000000000;
    nanos *= tp.tv_sec;
    return nanos + tp.tv_nsec;
}

void newOrder(Format &format, const char *id, int price, int quantity) {
    format.append("NEW ");
    format.append(id);
    format.append(' ');
    format.decimalAppend(price);
    format.append(' ');
    format.decimalAppendNonNeg(quantity);
    format.finish();
}

int main(int, const char **) {
    constexpr auto numIters = 100000000;
    Format buf;
    newOrder(buf, "TWTR", 0, 1);
    if (strcmp(buf.c_str(), "NEW TWTR 0 1")) {
        cout << buf.c_str() << endl;
        return 1;
    }
    buf.reset();
    newOrder(buf, "TWTR",-98760,102030);
    if (strcmp(buf.c_str(), "NEW TWTR -98760 102030")) {
        cout << buf.c_str() << endl;
        return 1;
    }
    auto begin = timestamp();
    for (int i = 0; i < numIters; ++i) {
        buf.reset();
        newOrder(buf, "TWTR", i & 0xff, (i & 0xfff) + 1);
    }
    auto nsTaken = timestamp() - begin;
    cout << numIters << " orders in " << nsTaken << "ns" << endl;
    cout << (double)nsTaken / numIters << "ns / order" << endl;
    return 0;
}
