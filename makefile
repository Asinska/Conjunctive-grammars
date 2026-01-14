CXX      := g++
CXXFLAGS := -O2 -std=c++20 -Wall -Wextra -I.
LDLIBS   := -lgmpxx -lgmp


LIB_SRC  := conjunctive_grammar/grammar.cc \
            conjunctive_grammar/grammar_io.cc \
            conjunctive_grammar/symbol_table.cc \
            conjunctive_grammar/symbol_names_generator.cc


SOLVER_SRC := solver/main.cc \
              solver/convolution.cc \
              solver/grammar_solver.cc


GRAMMAR_SRC := conjunctive_grammar/main.cc

LIB_OBJ     := $(LIB_SRC:.cc=.o)
SOLVER_OBJ  := $(SOLVER_SRC:.cc=.o)
GRAMMAR_OBJ := $(GRAMMAR_SRC:.cc=.o)


TARGET_SOLVER  := solver_main
TARGET_GRAMMAR := grammar_main


all: $(TARGET_SOLVER) $(TARGET_GRAMMAR)


$(TARGET_SOLVER): $(SOLVER_OBJ) $(LIB_OBJ)
	$(CXX) $(SOLVER_OBJ) $(LIB_OBJ) -o $@ $(LDLIBS)


$(TARGET_GRAMMAR): $(GRAMMAR_OBJ) $(LIB_OBJ)
	$(CXX) $(GRAMMAR_OBJ) $(LIB_OBJ) -o $@ $(LDLIBS)


%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(TARGET_SOLVER) $(TARGET_GRAMMAR) \
	      $(SOLVER_OBJ) $(GRAMMAR_OBJ) $(LIB_OBJ)

.PHONY: all clean