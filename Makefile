# Performance-tuning talk benchmarks.
#
# 'first' (Take One) is the binary we 'perf record' - the slide wants the
# original "20%+ __dynamic_cast" row at the top, which requires GCC <= 11
# (later versions inline or replace it). Built with GCC 11 + -static-libstdc++
# so the pre-fix code actually runs. Everything else uses GCC 16.

all: first first-modern second third fourth fifth sixth seventh eighth

CXXFLAGS = -std=c++23 -O2 -ggdb3 -Wall -Wextra -Werror

PRE_FIX_CXX := /opt/compiler-explorer/gcc-11.4.0/bin/g++
MODERN_CXX  := /opt/compiler-explorer/gcc-16.1.0/bin/g++

.PHONY: clean first first-modern second third fourth fifth sixth seventh eighth

clean:
	rm -rf bin

first: bin/first
bin/first: first.cpp bench.hpp
	@mkdir -p bin
	$(PRE_FIX_CXX) $(CXXFLAGS) -static-libstdc++ -static-libgcc \
		first.cpp -o bin/first

# Take One rebuilt with the modern toolchain - off-stage reference for
# "what does this look like once libstdc++ has the GCC 13 fix?"
first-modern: bin/first-modern
bin/first-modern: first.cpp bench.hpp
	@mkdir -p bin
	$(MODERN_CXX) $(CXXFLAGS) first.cpp -o bin/first-modern

second: bin/second
bin/second: second.cpp bench.hpp
	@mkdir -p bin
	$(MODERN_CXX) $(CXXFLAGS) second.cpp -o bin/second

third: bin/third
bin/third: third.cpp bench.hpp
	@mkdir -p bin
	$(MODERN_CXX) $(CXXFLAGS) third.cpp -o bin/third

fourth: bin/fourth
bin/fourth: fourth.cpp bench.hpp
	@mkdir -p bin
	$(MODERN_CXX) $(CXXFLAGS) fourth.cpp -o bin/fourth

fifth: bin/fifth
bin/fifth: fifth.cpp bench.hpp
	@mkdir -p bin
	$(MODERN_CXX) $(CXXFLAGS) fifth.cpp -o bin/fifth

sixth: bin/sixth
bin/sixth: sixth.cpp bench.hpp
	@mkdir -p bin
	$(MODERN_CXX) $(CXXFLAGS) sixth.cpp -o bin/sixth

seventh: bin/seventh
bin/seventh: seventh.cpp bench.hpp
	@mkdir -p bin
	$(MODERN_CXX) $(CXXFLAGS) seventh.cpp -o bin/seventh

eighth: bin/eighth
bin/eighth: eighth.cpp bench.hpp
	@mkdir -p bin
	$(MODERN_CXX) $(CXXFLAGS) eighth.cpp -lfmt -o bin/eighth
