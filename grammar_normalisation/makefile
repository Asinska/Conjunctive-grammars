CXX      := g++
CXXFLAGS := -O2 -std=c++20 -Wall -Wextra
LDLIBS   := -lgmpxx -lgmp

TARGET   := main

SRC      := main.cc grammar.cc grammar_io.cc symbol_table.cc symbol_names_generator.cc
OBJ      := $(SRC:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDLIBS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean
