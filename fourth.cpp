#include <cstdint>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

typedef uint64_t tstamp_t;

class Format {
    char _buffer[2048];
    static uint16_t _lookup[100];
    int _ptr;
public:
    static void init() {
        for (int i = 0; i < 100; ++i) {
            auto dig1 = '0' + (i % 10);
            auto dig2  = '0' + ((i/10) % 10);
            _lookup[i] = dig2 | dig1 << 8;
        }
    }
    Format() : _ptr(0) {}
    inline void reset() { _ptr = 0; }
    const char* c_str() const { return _buffer; }
    void append(char c) { _buffer[_ptr++] = c; }
    void finish() { append('\x00'); }

    void append(const char *data) {
        while (*data) append(*data++);
    }

    template<typename T>
    inline void decimalAppendNZ(T value) {
        int startPos = _ptr;
        while (value) {
            append(static_cast<char>(value % 10) + '0');
            value /= 10;
        }
        // Reverse the digits.
        int endPos = _ptr - 1;
        while (endPos > startPos) {
            std::swap(_buffer[endPos], _buffer[startPos]);
            ++startPos;
            --endPos;
        }
    }

    template<int N, typename T>
    inline void decimalPadAppend(T value) {
        for (int i = N - 1; i >= 0; --i) {
            char c = static_cast<char>(value % 10) + '0';
            value /= 10;
            _buffer[_ptr + i] = c;
        }
        _ptr += N;
    }

    template<int N, typename T>
    inline void decimalPadAppend2(T value) {
        static_assert((N % 2) == 0, "uh oh");
        for (int i = N - 2; i >= 0; i -= 2) {
            auto foo = value % 100;
            value /= 100;
            *reinterpret_cast<uint16_t *>(_buffer + _ptr + i) = _lookup[foo];
        }
        _ptr += N;
    }

    template<typename T>
    inline void decimalAppendUnsigned(T value) {
        if (value == 0) {
            append('0');
            return;
        }
        decimalAppendNZ(value);
    }

    template<typename T>
    inline void decimalAppend(T value) {
        if (value == 0) {
            append('0');
            return;
        } else if (value < 0) {
            append('-');
            value = -value;
        }
        decimalAppendNZ(value);
    }
};
uint16_t Format::_lookup[100];

static inline tstamp_t timestamp() {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    uint64_t nanos = 1000000000;
    nanos *= tp.tv_sec;
    return nanos + tp.tv_nsec;
}

void newOrder(Format &format, int id, int price, int quantity) {
    format.append("NEW ");
    format.decimalAppend(id);
    format.append(' ');
    format.decimalAppend(price);
    format.append(' ');
    format.decimalPadAppend2<4>(quantity);
    format.finish();
}

int main(int, const char **) {
    constexpr auto numIters = 100000000;
    Format buf;
    Format::init();
    newOrder(buf, 0,0,1);
    if (strcmp(buf.c_str(), "NEW 0 0 0001")) {
        cout << buf.c_str() << endl;
        return 1;
    }
    buf.reset();
    newOrder(buf, 1234,-98760,1020);
    if (strcmp(buf.c_str(), "NEW 1234 -98760 1020")) {
        cout << buf.c_str() << endl;
        return 1;
    }
    auto begin = timestamp();
    for (int i = 0; i < numIters; ++i) {
        buf.reset();
        newOrder(buf, i, i & 0xff, (i & 0xfff) + 1);
    }
    auto nsTaken = timestamp() - begin;
    cout << numIters << " orders in " << nsTaken << "ns" << endl;
    cout << (double)nsTaken / numIters << "ns / order" << endl;
    return 0;
}
