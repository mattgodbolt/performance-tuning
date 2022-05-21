all: first second third fourth fifth

CXXFLAGS = -std=c++0x -O2 -ggdb3

.PHONY: clean first second third fourth fifth

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
