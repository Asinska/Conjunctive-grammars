CXX      := g++
CXXFLAGS := -O2 -std=c++17 -Wall -Wextra
LDLIBS   := -lgmpxx -lgmp

TARGET   := main

SRC      := UnaryGrammarCheck.cpp parse_input.cpp helpers.cpp structures.cpp
OBJ      := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean
