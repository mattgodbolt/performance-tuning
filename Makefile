all: first second third fourth fifth sixth seventh eighth

CXXFLAGS = -std=c++23 -O2 -ggdb3 -Wall -Wextra -Werror

.PHONY: clean first second third fourth fifth sixth seventh eighth

clean:
	rm -rf bin

first: bin/first
bin/first: first.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) first.cpp -o bin/first

second: bin/second
bin/second: second.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) second.cpp -o bin/second

third: bin/third
bin/third: third.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) third.cpp -o bin/third

fourth: bin/fourth
bin/fourth: fourth.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) fourth.cpp -o bin/fourth

fifth: bin/fifth
bin/fifth: fifth.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) fifth.cpp -o bin/fifth

sixth: bin/sixth
bin/sixth: sixth.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) sixth.cpp -o bin/sixth

seventh: bin/seventh
bin/seventh: seventh.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) seventh.cpp -o bin/seventh

eighth: bin/eighth
bin/eighth: eighth.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) eighth.cpp -lfmt -o bin/eighth
