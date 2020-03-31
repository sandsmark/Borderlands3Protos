CCFILES=$(wildcard *.cpp)
CXXFLAGS += -ggdb -fPIC -std=c++17 -Wall -Wextra -pedantic
OBJECTS=$(patsubst %.cpp, %.o, $(CCFILES))
LDFLAGS=-g

decomp: $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f decomp $(OBJECTS)
