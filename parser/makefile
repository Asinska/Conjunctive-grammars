CXX      := g++
CXXFLAGS := -O2 -std=c++20 -Wall -Wextra
LDLIBS   := -lgmpxx -lgmp

TARGET   := main

SRC      := main.cc convolutions.cc grammar_solver.cc
OBJ      := $(SRC:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDLIBS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean
