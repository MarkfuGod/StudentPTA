CXX      ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -O2
CPPFLAGS += -Iinclude

APP  := student_pta
SRC  := $(wildcard src/*.cpp)
TEST := build/test_scoring

.PHONY: all run test clean

all: $(APP)

$(APP): $(SRC) $(wildcard include/*.h)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SRC) -o $@

run: $(APP)
	./$(APP)

$(TEST): tests/test_scoring.cpp src/scoring.cpp include/scoring.h include/record.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) tests/test_scoring.cpp src/scoring.cpp -o $@

test: $(TEST)
	./$(TEST) tests/golden_scores.csv

clean:
	rm -rf $(APP) build
