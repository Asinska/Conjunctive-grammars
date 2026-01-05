CXX      := g++
CXXFLAGS := -O2 -std=c++17 -Wall -Wextra
LDLIBS   := -lgmpxx -lgmp

TARGET   := main

SRC      := main.cpp read.cpp print.cpp structures.cpp helpers.cpp
OBJ      := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean
