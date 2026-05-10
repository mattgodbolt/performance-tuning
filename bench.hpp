#pragma once

// Minimal benchmarking helpers shared by first..eighth.cpp.
// Borrowed (and trimmed) from the harness in ../its-about-time/benchmarks.

#include <chrono>
#include <iostream>

namespace bench {

// Anti-optimisation barriers. do_not_optimize forces the compiler to treat
// `val` as observed (so it can't elide work that produces it); clobber memory
// for good measure.
template <typename T>
inline void do_not_optimize(const T &val) {
  asm volatile("" : : "r,m"(val) : "memory");
}

template <typename T>
inline void do_not_optimize(T &val) {
  asm volatile("" : "+r,m"(val) : : "memory");
}

// Time `fn(i)` over `iters` iterations using std::chrono::steady_clock and
// print "<iters> orders in Nns / X ns / order".
template <typename Fn>
void measure(long long iters, Fn &&fn) {
  const auto t0 = std::chrono::steady_clock::now();
  for (long long i = 0; i < iters; ++i)
    fn(i);
  const auto t1 = std::chrono::steady_clock::now();
  const auto ns =
      std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
  std::cout << iters << " orders in " << ns << "ns" << std::endl;
  std::cout << static_cast<double>(ns) / static_cast<double>(iters)
            << "ns / order" << std::endl;
}

} // namespace bench
