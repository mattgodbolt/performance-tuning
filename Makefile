# Performance-tuning talk benchmarks.
#
# Most binaries are built with a single modern toolchain ($(MODERN_CXX)) for
# apples-to-apples ratios. 'first' (Take One) is the exception: it's the
# binary we 'perf record', and the dynamic_cast hot path the slide investigates
# only exists in libstdc++ < 13 - so it's built with GCC 12 and statically
# links its own libstdc++. The other 'first-*' variants exist as off-stage
# references for the speaker (and curious audience members) - see the notes
# file for the 2x2 numbers.

all: first first-fixed first-dyn first-modern \
     second third fourth fifth sixth seventh eighth

CXXFLAGS = -std=c++23 -O2 -ggdb3 -Wall -Wextra -Werror

# Both fall back to $(CXX) if the targeted CE compiler isn't installed.
GCC12 := /opt/compiler-explorer/gcc-12.5.0/bin/g++
GCC16 := /opt/compiler-explorer/gcc-16.1.0/bin/g++
PRE_FIX_CXX  := $(if $(wildcard $(GCC12)),$(GCC12),$(CXX))
MODERN_CXX   := $(if $(wildcard $(GCC16)),$(GCC16),$(CXX))

STATIC_FLAGS := -static-libstdc++ -static-libgcc

.PHONY: clean first first-fixed first-dyn first-modern \
        second third fourth fifth sixth seventh eighth

clean:
	rm -rf bin

# --- the Take One matrix ---

first: bin/first
bin/first: first.cpp bench.hpp
	@mkdir -p bin
	$(PRE_FIX_CXX) $(CXXFLAGS) $(STATIC_FLAGS) first.cpp -o bin/first

first-fixed: bin/first-fixed
bin/first-fixed: first.cpp bench.hpp
	@mkdir -p bin
	$(MODERN_CXX) $(CXXFLAGS) $(STATIC_FLAGS) first.cpp -o bin/first-fixed

first-dyn: bin/first-dyn
bin/first-dyn: first.cpp bench.hpp
	@mkdir -p bin
	$(PRE_FIX_CXX) $(CXXFLAGS) first.cpp -o bin/first-dyn

first-modern: bin/first-modern
bin/first-modern: first.cpp bench.hpp
	@mkdir -p bin
	$(MODERN_CXX) $(CXXFLAGS) first.cpp -o bin/first-modern

# --- the rest: modern toolchain throughout ---

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
